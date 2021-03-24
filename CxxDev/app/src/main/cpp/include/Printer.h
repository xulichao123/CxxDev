//
// Created by xulichao on 2021/3/7.
//

#ifndef CXXDEV_PRINTER_H
#define CXXDEV_PRINTER_H

#include<android/log.h>
#include <iostream>


#define TAG    "Printer" // 这个是自定义的LOG的标识
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__) // 定义LOGD类型

class Printer {

public:
    template<class T>
    static void templatePrint(std::string tag, std::string msg, T data) {
        if (std::is_same<T, int>::value) {
            LOGD("CxxDev-%s / %s / data:%d", tag.c_str(), msg.c_str(), data);
        } else if (std::is_same<T, double>::value) {
            LOGD("CxxDev-%s / %s / data:%f", tag.c_str(), msg.c_str(), data);
        } else if (std::is_same<T, float>::value) {
            LOGD("CxxDev-%s / %s / data:%f", tag.c_str(), msg.c_str(), data);
        } else if (std::is_same<T, std::string>::value) {
            LOGD("CxxDev-%s / %s / data:%s", tag.c_str(), msg.c_str(), data);
        } else {
            LOGD("CxxDev-%s / %s / other type", tag.c_str(), msg.c_str());
        }
    }

};


#endif //CXXDEV_PRINTER_H
