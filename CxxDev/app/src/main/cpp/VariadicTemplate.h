//
// Created by xulichao on 2021/3/7.
//

#ifndef CXXDEV_VARIADICTEMPLATE_H
#define CXXDEV_VARIADICTEMPLATE_H

#define TAG "VariadicTemplate"

class VariadicTemplate {

private:
    void print();

    template<class T, class... Args>
    void print(T head, Args... rest);

    template<typename T>
    void typeCheck(T data);

public:
    void start();

private:

};


#endif //CXXDEV_VARIADICTEMPLATE_H
