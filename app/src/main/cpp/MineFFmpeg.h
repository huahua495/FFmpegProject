//
// Created by 77873 on 2020/7/16 0016.
//

#ifndef FFMPEGPROJECT_MINEFFMPEG_H
#define FFMPEGPROJECT_MINEFFMPEG_H

#include "MineCallJava.h"
#include "MineAudio.h"


#include "pthread.h"

extern "C" {
    #include <libavformat/avformat.h>
}


class MineFFmpeg {


public:
    MineCallJava *mineCallJava = NULL;
    const char *url = NULL;
    pthread_t  decodeThread;


    AVFormatContext *pFormatCtx=NULL;

    MineAudio *audio=NULL;


    MinePlayStatus *minePlayStatus=NULL;
public:

    MineFFmpeg(MineCallJava *callJava,const char *musicUrl,MinePlayStatus *playStatus);
    ~MineFFmpeg();


    void parpared();

    //解码线程
    void decodeFFmpegThread();

    void start();
};


#endif //FFMPEGPROJECT_MINEFFMPEG_H
