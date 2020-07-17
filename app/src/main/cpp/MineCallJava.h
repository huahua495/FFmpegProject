//
// Created by 77873 on 2020/7/16 0016.
//

#ifndef FFMPEGPROJECT_MINECALLJAVA_H
#define FFMPEGPROJECT_MINECALLJAVA_H

#include <cwchar>
#include "jni.h"
#include "AndroidLog.h"

#define  MAIN_THREAD 0
#define  CHILD_THREAD 1

class MineCallJava {


public:
    JavaVM *javaVM = NULL;

    JNIEnv *jniEnv = NULL;

    jobject jobj;

    jmethodID  jmid_parpared;

public:

    MineCallJava(JavaVM *vm, JNIEnv *evn, jobject *obj);

    ~MineCallJava();



    void onCallParpared(int type);

};


#endif //FFMPEGPROJECT_MINECALLJAVA_H
