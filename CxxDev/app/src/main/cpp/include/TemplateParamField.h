//
// Created by xulichao on 2021/3/6.
//

#ifndef CXXDEV_TEMPLATEPARAMFIELD_H
#define CXXDEV_TEMPLATEPARAMFIELD_H

#include<android/log.h>


#define TAG    "TemplateParamField" // 这个是自定义的LOG的标识
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__) // 定义LOGD类型


template<class T> class TemplateParamField {

private:
    T t;

public:

    TemplateParamField(T _t) : t(_t) {

    }

    void show() {
        LOGD("TemplateParamField / show / t: %s", t.c_str());
    }
};


#endif //CXXDEV_TEMPLATEPARAMFIELD_H
