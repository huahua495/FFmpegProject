//
// Created by 77873 on 2020/7/16 0016.
//

#include "MineAudio.h"


MineAudio::~MineAudio() {

}

MineAudio::MineAudio(MinePlayStatus *status) {
    this->playStatus = status;
    mineQueue = new MineQueue(playStatus);
    buffer = (uint8_t *) (av_malloc(44100 * 2 * 2));

}


void *decodePlay(void *data) {
    LOGE("MineAudio decodePlay");

    MineAudio *mineAudio = (MineAudio *) (data);
    /**
     * 使用OpenSL ES播放pcm数据时，调用初始化
     */
//    mineAudio->resampleAudio();

    mineAudio->initOpenSL();

    pthread_exit(&mineAudio->threadPlay);
}

/**
 * 重采样输出文件
 */
//FILE *outFile = fopen("/storage/emulated/0/ffmpeg/resampleMusic.pcm", "w");


void MineAudio::play() {
    pthread_create(&threadPlay, NULL, decodePlay, this);
}

int MineAudio::resampleAudio() {
    /**
     * 循环读取avPacket
     */

    LOGE("MineAudio::resampleAudio");


    if (playStatus) {
        LOGE("MineAudio::playStatus   is  null");
    }

    if (!playStatus->exit) {

        LOGE("MineAudio::playStatus   exit  is false");
    }


    while (playStatus != NULL && !playStatus->exit) {
        LOGE("MineAudio::while");
        avPacket = av_packet_alloc();

        if (mineQueue->outAvpacket(avPacket) != 0) {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            continue;
        }
        LOGE("outAvpacket");
        ret = avcodec_send_packet(avCodecContext, avPacket);

        if (ret != 0) {
            if (LOG_DEBUG) {
                LOGE("avcodec_send_packet on error");
            }

            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            continue;
        }
        LOGE("avcodec_send_packet");

        //为avFrame 开辟空间
        avFrame = av_frame_alloc();
        ret = avcodec_receive_frame(avCodecContext, avFrame);

        if (ret == 0) {
            /**
             * 有声道数 但是声道布局为0
             * 设置声道布局
             */
            if (avFrame->channels > 0 && avFrame->channel_layout == 0) {
                avFrame->channel_layout = av_get_default_channel_layout(avFrame->channels);

            } else if (avFrame->channels == 0 && avFrame->channel_layout > 0) {
                avFrame->channels = av_get_channel_layout_nb_channels(avFrame->channel_layout);
            }

            //重采样上下文
            SwrContext *swrContext = NULL;

            /**
             * struct SwrContext *s,
             * int64_t out_ch_layout,   //输出声道布局
             * enum AVSampleFormat out_sample_fmt,// 采样位数
             * int out_sample_rate,
             * int64_t  in_ch_layout,
             * enum AVSampleFormat  in_sample_fmt,
             * int  in_sample_rate,
             * int log_offset,
             * void *log_ctx
             */

            swrContext = swr_alloc_set_opts(NULL,
                                            AV_CH_LAYOUT_STEREO,
                                            AV_SAMPLE_FMT_S16,
                                            avFrame->sample_rate,
                                            avFrame->channel_layout,
                                            static_cast<AVSampleFormat>(avFrame->format),
                                            avFrame->sample_rate,
                                            NULL, NULL
            );
            if (LOG_DEBUG) {
                LOGE("swr_alloc_set_opts   ");
            }

            if (!swrContext ||
                swr_init(swrContext) < 0) {//如果失败进行初始化
                av_packet_free(&avPacket);
                av_free(avPacket);
                avPacket = NULL;

                av_frame_free(&avFrame);
                av_free(avFrame);
                avFrame = NULL;

                swr_free(&swrContext);
                continue;
            }

            if (LOG_DEBUG) {
                LOGE("swr_convert   ");
            }
            int nb = swr_convert(swrContext,
                                 &buffer,   //输出buffer
                                 avFrame->nb_samples,
                                 (const uint8_t **) (avFrame->data),
                                 avFrame->nb_samples
            );

            int outChannels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);

            data_size = nb * outChannels * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);

            if (LOG_DEBUG) {
                LOGE("fwrite 开始写入数据到文件 ");
            }
//            fwrite(buffer, 1, data_size, outFile);

            if (LOG_DEBUG) {
                LOGE("data  size is I%d", data_size);
            }

            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;

            av_frame_free(&avFrame);
            av_free(avFrame);
            avFrame = NULL;
            swr_free(&swrContext);
            swrContext = NULL;
            break;
        } else {
            if (LOG_DEBUG) {
                LOGE("avcodec_receive_frame on error");
            }
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;

            av_frame_free(&avFrame);
            av_free(avFrame);
            avFrame = NULL;
            continue;
        }
    }
//    fclose(outFile);
    LOGE("音频重采样结束");
    return data_size;
}

void pcmBufferCallBack(SLAndroidSimpleBufferQueueItf bf, void *context) {
    LOGE("MineAudio pcmBufferCallBack");

    MineAudio *mineAudio = (MineAudio *) (context);
    LOGE("%s", "pcmBufferCallBack  ");
    if (mineAudio != NULL) {
        int current_data_size = mineAudio->resampleAudio();
        if (current_data_size > 0) {
            (*mineAudio->pcmBufferQueue)->Enqueue(mineAudio->pcmBufferQueue,
                                                  (char *) mineAudio->buffer,
                                                  current_data_size);
        }
    } else {
        LOGE("%s", "pcmBufferCallBack  mineAudio  is null");
    }
}


void MineAudio::createEngine() {

    LOGE("MineAudio createEngine");
    SLresult result;

    // create engine
    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    if (SL_RESULT_SUCCESS != result) {

        LOGE("slCreateEngine  on error");
        return;
    };

    // realize the engine
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("realize the engine  on  error");
        return;
    }
    //get the engine interface, which is needed in order to create other objects
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &enginEngine);

    if (result != SL_RESULT_SUCCESS) {
        LOGE("get the engine interface  on error");
        return;
    }


    //create output mix, with environmental reverb specified as a non-required interface

    const SLInterfaceID mids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean mreq[1] = {SL_BOOLEAN_FALSE};


    result = (*enginEngine)->CreateOutputMix(enginEngine, &outputMixObject, 1, mids, mreq);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("CreateOutputMix  on  error")
        return;
    }

    //realize the output mix
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("realize the output mix  on error");
        return;
    }
    // get the environmental reverb interface
    result = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB,
                                              &outputMixEnviromentalReverb);

    if (result != SL_RESULT_SUCCESS) {
        LOGE("get the environmental reverb interface  on error")
        return;
    }

    result = (*outputMixEnviromentalReverb)->SetEnvironmentalReverbProperties(
            outputMixEnviromentalReverb, &reverbSettings);

    if (result != SL_RESULT_SUCCESS) {
        LOGE("SetEnvironmentalReverbProperties  on error ");
        return;
    }
}

void MineAudio::createBufferQueueAudioPlayer() {
    LOGE("MineAudio createBufferQueueAudioPlayer");

    SLresult result;
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM,
                                   2,//声道数
                                   SL_SAMPLINGRATE_44_1,//采样率
                                   SL_PCMSAMPLEFORMAT_FIXED_16,
                                   SL_PCMSAMPLEFORMAT_FIXED_16,
                                   SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
                                   SL_BYTEORDER_LITTLEENDIAN};
    SLDataSource audioSrc = {&loc_bufq, &format_pcm};

    // configure audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSnk = {&loc_outmix, NULL};


    const SLInterfaceID ids[1] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[1] = {SL_BOOLEAN_TRUE};


    result = (*enginEngine)->CreateAudioPlayer(enginEngine,
                                               &pcmPlayerObject, &audioSrc, &audioSnk,
                                               1, ids,
                                               req);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("CreateAudioPlayer  on error")
        return;
    }

    // realize the player
    result = (*pcmPlayerObject)->Realize(pcmPlayerObject, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("realize the player  on error")
        return;
    }
    // get the play interface
    result = (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_PLAY, &pcmPlayerPlay);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("get the play interface  on error")
        return;
    }
    // get the buffer queue interface
    result = (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_BUFFERQUEUE, &pcmBufferQueue);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("get the buffer queue interface  on error")
        return;
    }
    // register callback on the buffer queue
    result = (*pcmBufferQueue)->RegisterCallback(pcmBufferQueue, pcmBufferCallBack, this);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("register callback on the buffer queue  on error")
        return;
    }

    result = (*pcmPlayerPlay)->SetPlayState(pcmPlayerPlay, SL_PLAYSTATE_PLAYING);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("SetPlayState  on error")
        return;
    }
    /**
     * 回调
     */
    pcmBufferCallBack(pcmBufferQueue, this);
}

void MineAudio::initOpenSL() {
    LOGE("MineAudio initOpenSL");
    createEngine();

    createBufferQueueAudioPlayer();
}


