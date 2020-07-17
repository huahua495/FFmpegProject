//
// Created by 77873 on 2020/7/16 0016.
//

#include "MineCallJava.h"

MineCallJava::MineCallJava(JavaVM *vm, JNIEnv *env, jobject *obj) {

    this->javaVM = vm;
    this->jniEnv = env;
    this->jobj = *obj;
    this->jobj = env->NewGlobalRef(jobj);



    jclass  jlz = jniEnv->GetObjectClass(jobj);
    if (!jlz) {

        if (LOG_DEBUG) {
            LOGE("GetObjectClass is error")
        }
        return;
    }

    jmid_parpared = env->GetMethodID(jlz, "onCallParpared", "()V");

}

MineCallJava::~MineCallJava() {

}

void MineCallJava::onCallParpared(int type) {
    if(LOG_DEBUG){
        LOGE("MineCallJava::onCallParpared")
    }

    if (type == MAIN_THREAD) {
        //主线程
        jniEnv->CallVoidMethod(jobj, jmid_parpared);

        if(LOG_DEBUG){
            LOGE("主线程调用")
        }

    } else if (type == CHILD_THREAD) {
        if(LOG_DEBUG){
            LOGE("子线程调用")
        }
        JNIEnv *current_evn ;
        if (javaVM->AttachCurrentThread(&current_evn, 0)!=JNI_OK){
            if(LOG_DEBUG){
                LOGE("get child thread jnienv error");
                return;
            }
        };
        current_evn->CallVoidMethod(jobj, jmid_parpared);

        if(LOG_DEBUG){
            LOGE("CallVoidMethod")
        }
        javaVM->DetachCurrentThread();
    }

}

