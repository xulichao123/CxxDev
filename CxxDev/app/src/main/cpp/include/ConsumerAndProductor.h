//
// Created by xulichao on 2021/2/27.
//

#ifndef CXXDEV_CONSUMERANDPRODUCTOR_H
#define CXXDEV_CONSUMERANDPRODUCTOR_H

#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>
#include<android/log.h>

#define TAG    "ConsumerAndProductor" // 这个是自定义的LOG的标识
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__) // 定义LOGD类型


class ConsumerAndProductor {

public:
    ConsumerAndProductor();
    ~ConsumerAndProductor();
    void start();
};


#endif //CXXDEV_CONSUMERANDPRODUCTOR_H
