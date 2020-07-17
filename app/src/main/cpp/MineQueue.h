//
// Created by 77873 on 2020/7/16 0016.
//

#ifndef FFMPEGPROJECT_MINEQUEUE_H
#define FFMPEGPROJECT_MINEQUEUE_H

#include "queue"
#include "pthread.h"

#include "AndroidLog.h"
#include "MinePlayStatus.h"

extern "C" {
#include <libavcodec/avcodec.h>
};


class MineQueue {


public:
    std::queue<AVPacket *> queuePacket;

    pthread_mutex_t pthread_mutex;
    pthread_cond_t pthread_cond;

    MinePlayStatus *minePlayStatus=NULL;

public:
    MineQueue(MinePlayStatus *playStatus);
    ~MineQueue();


    /**
     * 入队
     */
    int inAvpacket(AVPacket *packet);

    /**
     * 出队
     */
    int outAvpacket(AVPacket *packet);


    /**
     * 获取队列大小
     */

    int getQueueSize();

};


#endif //FFMPEGPROJECT_MINEQUEUE_H
