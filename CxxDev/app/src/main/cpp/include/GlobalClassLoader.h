//
// Created by xulichao on 2021/3/19.
//

#ifndef CXXDEV_GLOBALCLASSLOADER_H
#define CXXDEV_GLOBALCLASSLOADER_H

#include <jni.h>

struct GlobalClassLoaderStruct
{
    jobject classLoader;
    jmethodID loadClass;
};

extern GlobalClassLoaderStruct GlobalClassLoader;


#endif //CXXDEV_GLOBALCLASSLOADER_H
