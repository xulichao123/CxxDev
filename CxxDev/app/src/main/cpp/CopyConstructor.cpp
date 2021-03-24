/**
 * 学习拷贝构造函数和赋值运算符
 */
#include <CopyConstructor.h>

CopyConstructor::CopyConstructor() {
    LOGD("Default Constructor");
}

CopyConstructor::~CopyConstructor() {
    LOGD("DeConstructor / userName.addr:%p", userName);
    if (userName != NULL) {
        delete userName;
        userName = NULL;
    }
}

CopyConstructor::CopyConstructor(const CopyConstructor& cc) {
    LOGD("Copy Constructor");
}

CopyConstructor::CopyConstructor(char *t_name) {
    userName = new char[strlen(t_name)+1];
    strcpy(userName, t_name);
}

CopyConstructor& CopyConstructor::operator=(const CopyConstructor& p) {
    LOGD("Assign");

    if (userName != NULL) {
        delete userName;
    }
    const int len = strlen(p.userName);
    userName = new char[len+1];
    strcpy(userName, p.userName);
    return *this; //返回
}

void CopyConstructor::f(CopyConstructor p) {
    return;
}

CopyConstructor CopyConstructor::f1() {
    CopyConstructor p;
    return p;
}