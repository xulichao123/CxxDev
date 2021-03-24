/**
 * 学习多线程 锁同步 和 条件变量配合实现 生产者消费者模式
 */

#include "include/ConsumerAndProductor.h"
#include <list>

static const int TOTAL = 20;//要生产的产品数目

std::mutex mtx;//互斥量，保护产品缓冲区

std::condition_variable mCondEmpty;//条件变量指示产品缓冲区不满
std::condition_variable mCondNoEmpty;//条件变量指示产品缓冲区不为空，就是缓冲区有产品

int mCurElement;

std::list<int> mList;

ConsumerAndProductor::ConsumerAndProductor() {
    LOGD("构造函数");
    mCurElement = 1;
}

ConsumerAndProductor::~ConsumerAndProductor() {
    LOGD("析构函数");
    mCurElement = 1;
    mList.clear();
}

int produce_item()
{
    std::unique_lock<std::mutex> lck(mtx);
    while (!mList.empty())
    {
        LOGD("Producer is waiting for an empty slot...");
        mCondEmpty.wait(lck);
    }

    const int last = mCurElement++;
    mList.push_back(last);
    LOGD("生产者生产第 %d 个产品", last);

    mCondNoEmpty.notify_all();//通知消费者产品库不为空

    lck.unlock();//解锁

    return last;
}

int consume_item()
{
    std::unique_lock<std::mutex> lck(mtx);
    while (mList.empty())
    {
        LOGD("Consumer is waiting for items...");
        mCondNoEmpty.wait(lck);// 消费者等待"产品库缓冲区不为空"这一条件发生.
    }

    const int last = mList.back();
    mList.pop_back();
    LOGD("消费者消费第 %d 个产品", last);

    mCondEmpty.notify_all();//通知产品库不满
    lck.unlock();

    return last;
}


void Producer_thread()
{
    while (true) {
        const int last = produce_item();
        if (last == TOTAL) {
            break;
        }
    }
}

void Consumer_thread()
{
    while (true)
    {
        const int item = consume_item();
        if (item == TOTAL) {
            break;
        }
    }
}

void ConsumerAndProductor::start() {
    LOGD("开始生产和消费");

    std::thread producer(Producer_thread); // 创建生产者线程.
    std::thread consumer(Consumer_thread); // 创建消费之线程.
    producer.join();
    consumer.join();
    LOGD("游戏结束");
}
