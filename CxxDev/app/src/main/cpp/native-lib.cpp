#include <jni.h>
#include <string>
#include<android/log.h>

#include "CopyConstructor.h"
#include "ConsumerAndProductor.h"
#include "TemplateTest.h"
#include "TemplateParamTest.h"
#include "TemplateParamField.h"
#include "VariadicTemplate.h"
#include "JniMultipleThreadAccess.h"
#include "jni_global.h"

#define TAG    "TestClass-C" // 这个是自定义的LOG的标识
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__) // 定义LOGD类型


jint JNI_OnLoad(JavaVM* jVM, void*)
{
    JNIGlobal.JNI_VERSION = JNI_VERSION_1_4;
    JNIGlobal.jVM = jVM;
    return JNIGlobal.JNI_VERSION;
}

/**
 * 反射调用java对象
 */
extern "C" JNIEXPORT jstring JNICALL
Java_com_autonavi_cxxdev_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
    std::string hello = "Hello from C++";

    //实例化Test类
    jclass testclass = env->FindClass("com/autonavi/cxxdev/TestClass");

    //构造函数的方法名为<init>
    jmethodID testcontruct = env->GetMethodID(testclass, "<init>", "()V");

    //根据构造函数实例化对象
    jobject testobject = env->NewObject(testclass, testcontruct);

    //调用成员方法，需使用jobject对象
    jmethodID test = env->GetMethodID(testclass, "test", "(I)V");
    env->CallVoidMethod(testobject, test, 100);

    //调用成员方法(带返回值)，需使用jobject对象
    jmethodID testReturn = env->GetMethodID(testclass, "testReturn", "(I)I");
    jint result = env->CallIntMethod(testobject, testReturn, 101);
    LOGD("testReturn / result = %d", result);

    return env->NewStringUTF(hello.c_str());
}

/**
 * java层对象传给c++
 */
extern "C" JNIEXPORT void JNICALL
Java_com_autonavi_cxxdev_MainActivity_setUserList(JNIEnv *env, jobject thiz, jobject userList) {
    int i;
    //class ArrayList
    jclass cls_arraylist = env->GetObjectClass(userList);
    //method in class ArrayList
    jmethodID arraylist_get = env->GetMethodID(cls_arraylist, "get", "(I)Ljava/lang/Object;");
    jmethodID arraylist_size = env->GetMethodID(cls_arraylist, "size", "()I");
    jint len = env->CallIntMethod(userList, arraylist_size);
    LOGD("get java ArrayList<User> object by C++ , then print it...../n");
    for (i = 0; i < len; i++) {
        jobject obj_user = env->CallObjectMethod(userList, arraylist_get, i);
        jclass cls_user = env->GetObjectClass(obj_user);
        jmethodID user_getId = env->GetMethodID(cls_user, "getId", "()J");
        jmethodID user_getUserName = env->GetMethodID(cls_user, "getUserName",
                                                      "()Ljava/lang/String;");
        jmethodID user_isMan = env->GetMethodID(cls_user, "isMan", "()Z");
        jmethodID user_getAge = env->GetMethodID(cls_user, "getAge", "()I");
        jstring name = (jstring) env->CallObjectMethod(obj_user, user_getUserName);
        jboolean b = true;
        const char *namePtr = env->GetStringUTFChars(name, &b);
        jlong id = env->CallLongMethod(obj_user, user_getId);
        jboolean sex = env->CallBooleanMethod(obj_user, user_isMan);
        jint age = env->CallIntMethod(obj_user, user_getAge);
        LOGD("UserItem / id:%d / name:%d / isMan:%d / Age:%d", id, namePtr, sex, age);
    }
}

/**
 * c++传对象给java层
 */
extern "C" JNIEXPORT jobject JNICALL
Java_com_autonavi_cxxdev_MainActivity_getUserList(JNIEnv *env, jobject thiz) {
    jclass cls_ArrayList = env->FindClass("java/util/ArrayList");
    jmethodID construct = env->GetMethodID(cls_ArrayList, "<init>", "()V");
    jobject obj_ArrayList = env->NewObject(cls_ArrayList, construct);
    jmethodID arrayList_add = env->GetMethodID(cls_ArrayList, "add", "(Ljava/lang/Object;)Z");
    //User Object
    jclass cls_user = env->FindClass("com/autonavi/cxxdev/User");
    //none argument construct function
    jmethodID construct_user = env->GetMethodID(cls_user, "<init>", "()V");
    //new a object
    jobject obj_user = env->NewObject(cls_user, construct_user);

    int i;
    for (i = 0; i < 10; i++) {
        //new a object
        jobject obj_user = env->NewObject(cls_user, construct_user);
        //get field id
        jfieldID user_id = env->GetFieldID(cls_user, "id", "J");
        jfieldID user_name = env->GetFieldID(cls_user, "userName", "Ljava/lang/String;");
        jfieldID user_isMan = env->GetFieldID(cls_user, "isMan", "Z");
        jfieldID user_age = env->GetFieldID(cls_user, "age", "I");
        env->SetLongField(obj_user, user_id, i);
        env->SetObjectField(obj_user, user_name, env->NewStringUTF("xulc"));
        env->SetBooleanField(obj_user, user_isMan, 1);
        env->SetIntField(obj_user, user_age, 21);
        env->CallBooleanMethod(obj_ArrayList, arrayList_add, obj_user);
    }
    return obj_ArrayList;
}

/**
 * c++拷贝构造函数
 */
extern "C" JNIEXPORT void JNICALL
Java_com_autonavi_cxxdev_MainActivity_copyConstructor(JNIEnv *env, jobject thiz) {
    //通过new来创建对象
    //CopyConstructor* cc = new CopyConstructor();
    //delete cc;

    //创建对象实例（退出函数作用域自动销毁）
    //CopyConstructor obj;

    //通过拷贝构造函数创建新对象
    //CopyConstructor p;
    //CopyConstructor q(p);

    //重载赋值运算符
    //CopyConstructor p1;
    //CopyConstructor p2;
    //p2 = p1;

    //测试默认赋值导致二次析构问题
    CopyConstructor p1("hello world");
    CopyConstructor p2;
    p2 = p1;
}

/**
 * c++生产者和消费者
 */
extern "C" JNIEXPORT void JNICALL
Java_com_autonavi_cxxdev_MainActivity_consumerAndProductor(JNIEnv *env, jobject thiz) {
    LOGD("consumerAndProductor / start");
    ConsumerAndProductor cp;
    cp.start();
    LOGD("consumerAndProductor / end");
}

/**
 * C++ 模版使用案例
 */
extern "C" JNIEXPORT void JNICALL
Java_com_autonavi_cxxdev_MainActivity_templateTest(JNIEnv *env, jobject thiz) {
    TemplateTest<int> t(11, 100);
    t.show();
}

/**
 * C++ 模版参数使用案例
 */
extern "C" JNIEXPORT void JNICALL
Java_com_autonavi_cxxdev_MainActivity_templateParam(JNIEnv *env, jobject thiz) {
    TemplateParamField<std::string> container("hello cxx");
    TemplateParamTest<std::string, TemplateParamField> test("hello world", container);
    test.container.show();
}

/**
 * C++ 可变参数模版使用案例
 */
extern "C" JNIEXPORT void JNICALL
Java_com_autonavi_cxxdev_MainActivity_variadicTemplate(JNIEnv *env, jobject thiz) {
    VariadicTemplate vt;
    vt.start();
}

/**
 * 多线程访问(解决ClassLoader找不到问题)
 */
extern "C" JNIEXPORT void JNICALL
Java_com_autonavi_cxxdev_MainActivity_multipleThreadAccess(JNIEnv *env, jobject thiz) {

}

/**
 * 多线程访问(解决ClassLoader找不到问题)
 */
extern "C" void
Java_com_autonavi_cxxdev_MainActivity_multipleThreadClassNotFound(JNIEnv *env, jobject thiz) {
    JniMultipleThreadAccess thread;
    thread.initGlobalClassLoader(env);
    thread.startThreadCallGlobalClassLoader();
}

/**
 * 多线程访问(解决ClassLoader找不到问题)
 */
extern "C" void
Java_com_autonavi_cxxdev_MainActivity_startAttachCurrentThread(JNIEnv *env, jobject thiz) {
    JniMultipleThreadAccess thread;
    thread.startAttachCurrentThread(env, thiz);
}

/**
 * JNI层持有对象和Java层生命周期不一致导致崩溃
 */

jobject mUser;

extern "C" void
Java_com_autonavi_cxxdev_MainActivity_setUser(JNIEnv *env, jobject thiz, jobject user) {
    if (user != nullptr) {
        mUser = user;
    }

    jclass userClass = env->GetObjectClass(mUser);
    jmethodID getNameMtdId = env->GetMethodID(userClass, "getUserName", "()Ljava/lang/String;");
    jobject userName = env->CallObjectMethod(mUser, getNameMtdId);
}


/**
 * Native调用Java代码时出现异常
 */
extern "C" void
Java_com_autonavi_cxxdev_MainActivity_callJavaException(JNIEnv *env, jobject thiz, jobject user) {
    //jobject activity = env->NewLocalRef(thiz);

    jclass userClass = env->GetObjectClass(user);
    jmethodID getNameMtdId = env->GetMethodID(userClass, "getAge", "()I");
    jint result = env->CallIntMethod(user, getNameMtdId);

    jthrowable exc = env->ExceptionOccurred();
    if (exc) {
        // 打印异常日志
        env->ExceptionDescribe();
        // 这行代码才是关键不让应用崩溃的代码，
        env->ExceptionClear();
        // 发生异常了要记得释放资源
        env->DeleteLocalRef(userClass);
    }
}

/**
 * Native 抛出 Java 中的异常
 */
extern "C" void
Java_com_autonavi_cxxdev_MainActivity_nativeThrowException(JNIEnv *env, jobject thiz) {
    jclass cls = env->FindClass("java/lang/IllegalArgumentException");
    if (cls != NULL) {
        env->ThrowNew(cls, "native throw exception");
    }
    env->DeleteLocalRef(cls);
}

/**
 * 来验证MonitorEnter 和 MonitorExit的用法
 * 证明：Java和Native端可以共用同一把锁
 */
extern "C" void
Java_com_autonavi_cxxdev_MainActivity_nativeSetMonitorData(JNIEnv *env, jobject thiz) {
    jclass cls = env->GetObjectClass(thiz);
    jfieldID fieldID = env->GetFieldID(cls, "mMonitorData", "I");
    if (env->MonitorEnter(thiz) != JNI_OK) {
        LOGD("%s: nativeSetMonitorData() failed", __FUNCTION__);
    }

    /* synchronized block */
    int val = env->GetIntField(thiz, fieldID);
    val++;
    LOGD("nativeSetMonitorData / mMonitorData=%d", val);
    env->SetIntField(thiz, fieldID, val);

    if (env->ExceptionOccurred()) {
        LOGD("nativeSetMonitorData / ExceptionOccurred()...");
        if (env->MonitorExit(thiz) != JNI_OK) {
            LOGD("%s: MonitorExit() failed", __FUNCTION__);
        };
    }

    if (env->MonitorExit(thiz) != JNI_OK) {
        LOGD("%s: MonitorExit() failed", __FUNCTION__);
    };
}
