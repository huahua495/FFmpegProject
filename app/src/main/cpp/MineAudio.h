//
// Created by 77873 on 2020/7/16 0016.
//

#ifndef FFMPEGPROJECT_MINEAUDIO_H
#define FFMPEGPROJECT_MINEAUDIO_H

#include "MineQueue.h"
#include "MinePlayStatus.h"


#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
}

class MineAudio {


public:

    /**
     * 流的索引
     */
    int streamIndex = -1;

    AVCodecParameters *codecParameters = NULL;

    AVCodecContext *avCodecContext = NULL;

    MineQueue *mineQueue = NULL;

    MinePlayStatus *playStatus = NULL;


    /**
     * 播放器线程
     */
    pthread_t threadPlay;

    AVPacket *avPacket = NULL;

    AVFrame *avFrame = NULL;


    int ret = -1;


    uint8_t *buffer = NULL;   //无符号8位类型

    int data_size = 0;


    SLObjectItf engineObject = NULL;

    SLEngineItf enginEngine = NULL;

    SLObjectItf outputMixObject = NULL;

    SLEnvironmentalReverbItf outputMixEnviromentalReverb = NULL;

    SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;


    SLObjectItf pcmPlayerObject = NULL;
    SLPlayItf pcmPlayerPlay = NULL;
    SLAndroidSimpleBufferQueueItf pcmBufferQueue = NULL;

public:

    MineAudio(MinePlayStatus *status);

    ~MineAudio();


    void play();


    /**
     * 重采样方法（返回重采样的大小）
     */
    int resampleAudio();


    void initOpenSL();

    void createEngine();

    void createBufferQueueAudioPlayer();

};


#endif //FFMPEGPROJECT_MINEAUDIO_H
