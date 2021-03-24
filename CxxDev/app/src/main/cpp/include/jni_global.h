#ifndef __JNI_GLOBAL_H__
#define __JNI_GLOBAL_H__

#include <jni.h>
#include <android/log.h>
#include <assert.h>
#include <stdio.h>


struct JNIGlobalStruct
{
	jint JNI_VERSION;

	JavaVM* jVM;
};

extern JNIGlobalStruct JNIGlobal;

#endif
