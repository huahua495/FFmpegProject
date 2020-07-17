#include <jni.h>
#include <string>

#include "MineFFmpeg.h"

// 有坑，会报错，必须混合编译
extern "C" {
#include <libavformat/avformat.h>
}


JavaVM *javaVM = NULL;
MineCallJava *callJava = NULL;
MineFFmpeg *ffmpeg;
MinePlayStatus *playStatus;



//通过JNI_OnLoad  初始化JavaVM
extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *resversed) {
    jint result = -1;

    JNIEnv *env;
    javaVM = vm;

    if (vm->GetEnv((void **) (&env), JNI_VERSION_1_6) != JNI_OK) {
        return result;
    }

    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_renfei_ffmpegproject_Player_MinePlayer_n_1parpared(JNIEnv *env, jobject instance,
                                                            jstring source_) {
    const char *source = env->GetStringUTFChars(source_, 0);




    if (NULL == ffmpeg) {
        if (NULL == callJava) {
            callJava = new MineCallJava(javaVM, env, &instance);
        }
        playStatus=new MinePlayStatus();

        ffmpeg = new MineFFmpeg(callJava, source,playStatus);
        ffmpeg->parpared();
    }

//    env->ReleaseStringUTFChars(source_, source);
}

/**
 * 开始播放
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_renfei_ffmpegproject_Player_MinePlayer_n_1start(JNIEnv *env, jobject instance) {


    if(ffmpeg!=NULL){
        ffmpeg->start();
    }
}