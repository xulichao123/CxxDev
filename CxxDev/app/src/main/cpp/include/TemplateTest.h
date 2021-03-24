//
// Created by xulichao on 2021/3/4.
//

#ifndef CXXDEV_TEMPLATETEST_H
#define CXXDEV_TEMPLATETEST_H

#include<android/log.h>

#define TAG    "TemplateTest" // 这个是自定义的LOG的标识
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__) // 定义LOGD类型


template<class T> class TemplateTest {

private:
    template<class V> class Hold {

    private:
        V val;

    public:
        Hold(V _val = 0) : val(_val) {

        }

        void show() {
            LOGD("Hold / show / val: %d", val);
        }

        V value() {
            return val;
        }
    };

private:
    Hold<T> mHold;
    Hold<int> mHold1;

public:

    TemplateTest(T t, int i)  : mHold(t), mHold1(i) {

    }

    void show() {
        mHold.show();
        mHold1.show();
    }
};


#endif //CXXDEV_TEMPLATETEST_H
