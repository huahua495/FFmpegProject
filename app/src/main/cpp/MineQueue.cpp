//
// Created by 77873 on 2020/7/16 0016.
//

#include "MineQueue.h"


MineQueue::MineQueue(MinePlayStatus *playStatus) {
    this->minePlayStatus = playStatus;

    pthread_mutex_init(&pthread_mutex, NULL);
    pthread_cond_init(&pthread_cond, NULL);
}


MineQueue::~MineQueue() {

    pthread_mutex_destroy(&pthread_mutex);
    pthread_cond_destroy(&pthread_cond);
}

int MineQueue::inAvpacket(AVPacket *packet) {
    pthread_mutex_lock(&pthread_mutex);

    queuePacket.push(packet);
    if (LOG_DEBUG) {
        LOGD("放入一个AVPacket 到队列中，个数为:%d", queuePacket.size());
    }

    pthread_cond_signal(&pthread_cond);

    pthread_mutex_unlock(&pthread_mutex);
    return 0;
}

int MineQueue::outAvpacket(AVPacket *packet) {

    pthread_mutex_lock(&pthread_mutex);


    while (minePlayStatus != NULL && !minePlayStatus->exit) {
        if (queuePacket.size() > 0) {

            AVPacket *avPacket=queuePacket.front();

            /**
             * 拷贝操作
             * 将packet 的值拷贝到 avPacket中
             *
             */
            if(av_packet_ref(packet,avPacket)==0){
                queuePacket.pop();
            }

            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket=NULL;

            if(LOG_DEBUG){
//                LOGE("从队列中取出一个avPacket  ,还剩下  %d 个 ",queuePacket.size());
            }
            break;
        } else {

            pthread_cond_wait(&pthread_cond, &pthread_mutex);
        }
    }

    pthread_mutex_unlock(&pthread_mutex);

    return 0;
}

int MineQueue::getQueueSize() {
    int size=0;

    pthread_mutex_lock(&pthread_mutex);

    size=queuePacket.size();

    pthread_mutex_unlock(&pthread_mutex);

    return size;
}



