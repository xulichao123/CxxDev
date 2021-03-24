//
// Created by xulichao on 2021/2/15.
//

#ifndef CXXDEV_COPYCONSTRUCTOR_H
#define CXXDEV_COPYCONSTRUCTOR_H

#include <iostream>
#include<android/log.h>

#define TAG    "CopyConstructor-C" // 这个是自定义的LOG的标识
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__) // 定义LOGD类型


using namespace std;

class CopyConstructor {

public:
    CopyConstructor();
    ~CopyConstructor();

    CopyConstructor(const CopyConstructor& cc);

    CopyConstructor(char* t_name);

    CopyConstructor& operator=(const CopyConstructor& p);

    void f(CopyConstructor p);

    CopyConstructor f1();

private:
    char* userName = NULL;
    pthread_mutex_t _lock = PTHREAD_MUTEX_INITIALIZER;
};

#endif //CXXDEV_COPYCONSTRUCTOR_H
