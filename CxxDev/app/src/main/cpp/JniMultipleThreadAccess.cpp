//
// Created by xulichao on 2021/3/18.
//

#include "include/JniMultipleThreadAccess.h"
#include<android/log.h>
#include <string>

#include "GlobalClassLoader.h"


#define TAG    "JniMultipleThreadAccess" // 这个是自定义的LOG的标识
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__) // 定义LOGD类型

jobject mGlobalVar = nullptr;


/**
 * 在自己创建的线程(类似通过pthread_create)中调用FindClass会失败得到空的返回，从而导致调用失败。
 * @return
 */
void* _testClassNotfound(void*) {
    JNIEnv *env = nullptr;
    int ret;

    JavaVMAttachArgs args;
    args.version = JNI_VERSION_1_4;
    args.name = "JniMultipleThreadAccess";//给线程起个名字吧，这样在调试或者崩溃的时候能显示出名字，而不是thead-1,thread-2这样的名字。
    args.group = NULL;//java.lang.ThreadGroup的全局引用，作用你懂的。

    //调用AttachCurrentThread，将当前线程附加到虚拟机，附加成功后，将会返回JNIEnv
    ret = JNIGlobal.jVM->AttachCurrentThread(&env, &args);
    LOGD("1 | AttachCurrentThread env is:%p, ret=%d", env, ret);

    //实例化Test类
    jclass testclass = env->FindClass("com/autonavi/cxxdev/TestClass");
    //构造函数的方法名为<init>
    jmethodID testcontruct = env->GetMethodID(testclass, "<init>", "()V");
    //根据构造函数实例化对象
    jobject testobject = env->NewObject(testclass, testcontruct);

    mGlobalVar = env->NewGlobalRef(testobject);
}

/**
 * 全局先缓存下可以访问到ClassLoader对象
 * @param env
 */
void _setGlobalClassLoader(JNIEnv* env) {
    jclass classLoaderClass = env->FindClass("java/lang/ClassLoader");
    jclass testClass = env->FindClass("com/autonavi/cxxdev/TestClass");
    if (testClass)
    {
        jmethodID getClassLoader = env->GetStaticMethodID(testClass, "getClassLoader", "()Ljava/lang/ClassLoader;");
        jobject obj = env->CallStaticObjectMethod(testClass, getClassLoader);
        GlobalClassLoader.classLoader = env->NewGlobalRef(obj);
        GlobalClassLoader.loadClass = env->GetMethodID(classLoaderClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");


        env->DeleteLocalRef(classLoaderClass);
        env->DeleteLocalRef(testClass);
        env->DeleteLocalRef(obj);
    }
}

jclass getClass(JNIEnv *env, const char* className) {
    jstring classNameString = env->NewStringUTF(className);
    jclass ret = (jclass)env->CallObjectMethod(GlobalClassLoader.classLoader, GlobalClassLoader.loadClass, classNameString);
    env->DeleteLocalRef(classNameString);
    return ret;
}

/**
 * 验证通过已经缓存好的ClassLoader来为pthread线程创建的JNIENV调用
 * @return
 */
void* _testCallGlobalClassLoader(void*) {
    JNIEnv *env = nullptr;
    int ret;

    JavaVMAttachArgs args;
    args.version = JNI_VERSION_1_4;
    args.name = "JniMultipleThreadAccess";//给线程起个名字吧，这样在调试或者崩溃的时候能显示出名字，而不是thead-1,thread-2这样的名字。
    args.group = NULL;//java.lang.ThreadGroup的全局引用，作用你懂的。

    //调用AttachCurrentThread，将当前线程附加到虚拟机，附加成功后，将会返回JNIEnv
    ret = JNIGlobal.jVM->AttachCurrentThread(&env, &args);
    LOGD("1 | _testCallGlobalClassLoader / env is:%p, ret=%d", env, ret);

    jclass testClass = getClass(env, "com/autonavi/cxxdev/TestClass");
    jmethodID testcontruct = env->GetMethodID(testClass, "<init>", "()V");
    //根据构造函数实例化对象
    jobject testobject = env->NewObject(testClass, testcontruct);

    env->DeleteLocalRef(testobject);
    env->DeleteLocalRef(testClass);

    LOGD("2 | _testCallGlobalClassLoader / env is:%p, ret=%d", env, ret);

    return nullptr;
}

/**
 * 验证局部变量溢出
 * @return
 */
void* _testOverflow(void*) {

    JNIEnv *env1 = nullptr;
    int ret;

    JavaVMAttachArgs args;
    args.version = JNI_VERSION_1_4;
    args.name = "JniMultipleThreadAccess";//给线程起个名字吧，这样在调试或者崩溃的时候能显示出名字，而不是thead-1,thread-2这样的名字。
    args.group = NULL;//java.lang.ThreadGroup的全局引用，作用你懂的。

    //调用AttachCurrentThread，将当前线程附加到虚拟机，附加成功后，将会返回JNIEnv
    ret = JNIGlobal.jVM->AttachCurrentThread(&env1, &args);
    LOGD("1 | AttachCurrentThread env is:%p, ret=%d", env1, ret);

    bool isTrue = true;
    std::string hello("Hello");
    while (isTrue) {
        jstring element = env1->NewStringUTF(hello.c_str());
        LOGD("2 | AttachCurrentThread while");
        //env1->DeleteLocalRef(element);
    }
    return nullptr;
}

/**
 * 验证全局变量溢出
 * @return
 */
void _testOverflowGlobal(JNIEnv* env) {

    bool isTrue = true;
    while (isTrue) {

        //实例化Test类
        jclass testclass = env->FindClass("com/autonavi/cxxdev/TestClass");
        //构造函数的方法名为<init>
        jmethodID testcontruct = env->GetMethodID(testclass, "<init>", "()V");
        //根据构造函数实例化对象
        jobject testobject = env->NewObject(testclass, testcontruct);

        mGlobalVar = env->NewGlobalRef(testobject);
        LOGD("2 | AttachCurrentThread while");

        //env->DeleteGlobalRef(mGlobalVar);
    }
}

static void destroy_key(void * p) {}

/**
 * 测试pthread_key 是有上限的，所以我们的代码中会调用pthread_once保证不会出现问题(居然没有崩溃，跑了好久)
 * @param env
 */
void _testPthreadKeyOverFlow(JNIEnv* env) {
    bool isTrue = true;
    while (isTrue) {
        LOGD("_testPthreadKeyOverFlow / pthread_key_create while");
        pthread_key_t sTLSKey;
        pthread_key_create(&sTLSKey, destroy_key);
        pthread_setspecific(sTLSKey, env);
    }
}



void JniMultipleThreadAccess::startThread() {
    pthread_t thread;
    pthread_create(&thread, NULL, _testClassNotfound, NULL);
}

void JniMultipleThreadAccess::startThread(JNIEnv *env) {
    //_testOverflowGlobal(env);
    _testPthreadKeyOverFlow(env);
}

void JniMultipleThreadAccess::initGlobalClassLoader(JNIEnv *env) {
    _setGlobalClassLoader(env);
}

void JniMultipleThreadAccess::startThreadCallGlobalClassLoader() {
    pthread_t thread;
    pthread_create(&thread, NULL, _testCallGlobalClassLoader, NULL);
}


jobject mActivity = nullptr;

void* _testAttachCurrentThread(void*) {
    JNIEnv *env;
    int ret;

    JavaVMAttachArgs args;
    args.version = JNI_VERSION_1_4;
    args.name = "JniMultipleThreadAccess";//给线程起个名字吧，这样在调试或者崩溃的时候能显示出名字，而不是thead-1,thread-2这样的名字。
    args.group = NULL;//java.lang.ThreadGroup的全局引用，作用你懂的。


    //调用AttachCurrentThread，将当前线程附加到虚拟机，附加成功后，将会返回JNIEnv
    ret = JNIGlobal.jVM->AttachCurrentThread(&env, &args);
    LOGD("_testAttachCurrentThread env is:%p, ret=%d", env, ret);


    jclass actClass = env->GetObjectClass(mActivity);
    jmethodID mtdFinish = env->GetMethodID(actClass, "finish", "()V");
    env->CallVoidMethod(mActivity, mtdFinish);

    env->DeleteLocalRef(actClass);
    env->DeleteGlobalRef(mActivity);
    //从虚拟机分离线程
    JNIGlobal.jVM->DetachCurrentThread();
    return NULL;
}

void JniMultipleThreadAccess::startAttachCurrentThread(JNIEnv *env, jobject thiz) {
    mActivity = env->NewGlobalRef(thiz);
    //直接引用会出现崩溃，必须通过NewGlobalRef保护
    //mActivity = thiz;

    pthread_t thread;
    pthread_create(&thread, NULL, _testAttachCurrentThread, NULL);
}