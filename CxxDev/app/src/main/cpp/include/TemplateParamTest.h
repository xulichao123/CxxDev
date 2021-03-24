//
// Created by xulichao on 2021/3/6.
//

#ifndef CXXDEV_TEMPLATEPARAMTEST_H
#define CXXDEV_TEMPLATEPARAMTEST_H


template<class T, template<class U>class Container> class TemplateParamTest {

public:
    Container<T> container;

    TemplateParamTest(T t, Container<T> _container) : container(_container) {

    }
};


#endif //CXXDEV_TEMPLATEPARAMTEST_H
