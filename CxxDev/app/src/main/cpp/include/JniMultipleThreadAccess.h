//
// Created by xulichao on 2021/3/18.
//

#ifndef CXXDEV_JNIMULTIPLETHREADACCESS_H
#define CXXDEV_JNIMULTIPLETHREADACCESS_H

#include <jni.h>
#include <pthread.h>
#include <android/log.h>
#include "jni_global.h"

class JniMultipleThreadAccess {

public:
    void startThread();
    void startThread(JNIEnv* env);

    //验证全局缓存ClassLoader，以便通过C++的pthread创建线程来调用
    void initGlobalClassLoader(JNIEnv* env);
    void startThreadCallGlobalClassLoader();

    void startAttachCurrentThread(JNIEnv *env, jobject thiz);

private:
    JavaVM *g_vm;
};


#endif //CXXDEV_JNIMULTIPLETHREADACCESS_H
