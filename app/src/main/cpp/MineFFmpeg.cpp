//
// Created by 77873 on 2020/7/16 0016.
//

#include "MineFFmpeg.h"


MineFFmpeg::MineFFmpeg(MineCallJava *callJava, const char *musicUrl, MinePlayStatus *playStatus) {
    this->mineCallJava = callJava;
    this->url = musicUrl;
    this->minePlayStatus = playStatus;
}


void *decodeFFmpeg(void *data) {
    MineFFmpeg *mineFFmpeg = (MineFFmpeg *) data;
    mineFFmpeg->decodeFFmpegThread();
    pthread_exit(&mineFFmpeg->decodeThread);
}

void MineFFmpeg::parpared() {
    pthread_create(&decodeThread, NULL, decodeFFmpeg, this);

}

void MineFFmpeg::decodeFFmpegThread() {

    if (LOG_DEBUG) {
        LOGE("decodeFFmpegThread  start");
    }

    av_register_all();
    avformat_network_init();

    pFormatCtx = avformat_alloc_context();

    if (avformat_open_input(&pFormatCtx, url, NULL, NULL) != 0) {

        if (LOG_DEBUG) {
            LOGD("avformat_open_input error ");
        }
        return;
    }

    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {

        if (LOG_DEBUG) {
            LOGD("avformat_find_stream_info error");
        }
        return;
    }

    /**
     * 根据pFormatCtx 遍历查找 获取对应的流
     */
    for (int i = 0; i < pFormatCtx->nb_streams; ++i) {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {

            if (audio == NULL) {

                audio = new MineAudio(minePlayStatus);
                audio->streamIndex = i;
                audio->codecParameters = pFormatCtx->streams[i]->codecpar;
            }
        }

    }

    /**
     * 根据流去查找解码器
     */
    AVCodec *dec = avcodec_find_decoder(audio->codecParameters->codec_id);

    if (!dec) {
        if (LOG_DEBUG) {
            LOGE("avcodec_find_decoder error");
        }
        return;
    }

    /**
     * 获取解码器的上下文对象
     */
    audio->avCodecContext = avcodec_alloc_context3(dec);

    if (!audio->avCodecContext) {

        if (LOG_DEBUG) {
            LOGE("avcodec_alloc_context3   error");
        }
        return;
    }
    /**
     *根据提供的编解码器中的值填充编解码器上下文参数。
     */
    if (avcodec_parameters_to_context(audio->avCodecContext,
                                      audio->codecParameters) < 0) {

        if (LOG_DEBUG) {
            LOGE("avcodec_parameters_to_context error");
        }
        return;
    }

    if (avcodec_open2(audio->avCodecContext, dec, 0) != 0) {
        if (LOG_DEBUG) {
            LOGE("avcodec_open2 error");
        }
        return;
    }


    if (NULL == mineCallJava) {
        if (LOG_DEBUG) {
            LOGE("decodeFFmpegThread  mineCallJava  is NULL");
        }
        return;
    }

    mineCallJava->onCallParpared(CHILD_THREAD);

}

void MineFFmpeg::start() {

    if (LOG_DEBUG) {
        LOGE("MineFFmpeg  start");
    }

    if (audio == NULL) {

        if (LOG_DEBUG) {

            LOGE("audio  is NULL");
        }
        return;
    }

    audio->play();

    int count = 0;

    while (minePlayStatus != NULL && !minePlayStatus->exit) {
        AVPacket *avPacket = av_packet_alloc();

        if (av_read_frame(pFormatCtx, avPacket) == 0) {

            if (avPacket->stream_index == audio->streamIndex) {

                count++;

                if (LOG_DEBUG) {

//                    LOGE("avPacket解码 %d 帧", count);
                }
                audio->mineQueue->inAvpacket(avPacket);
            } else {
                av_packet_free(&avPacket);
                av_free(avPacket);
            }
        } else {

            if (LOG_DEBUG) {
                LOGE("decode  finished ");
            }
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;

            while (minePlayStatus != NULL && !minePlayStatus->exit) {
                if (audio->mineQueue->getQueueSize() > 0) {
                    continue;
                } else {
                    minePlayStatus->exit = true;
                    break;
                }
            }
        }
    }

    if (LOG_DEBUG) {

        LOGE("解码完成");
    }

}
