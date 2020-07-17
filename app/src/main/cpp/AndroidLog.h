//
// Created by yangw on 2018-2-24.
//

#ifndef JNITHREAD_ANDROIDLOG_H_H
#define JNITHREAD_ANDROIDLOG_H_H

#endif //JNITHREAD_ANDROIDLOG_H_H

#include <android/log.h>

//宏定义后面不能带分号(会报错)
#define LOG_DEBUG true

#define LOGD(FORMAT,...) __android_log_print(ANDROID_LOG_DEBUG, "Renfei666", FORMAT, ##__VA_ARGS__);
#define LOGE(FORMAT,...) __android_log_print(ANDROID_LOG_ERROR, "Renfei666", FORMAT, ##__VA_ARGS__);

