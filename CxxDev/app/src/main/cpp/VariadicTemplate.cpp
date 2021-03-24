/**
 * 参考文档： https://ouonline.net/cpp-variadic-template
 */

#include "VariadicTemplate.h"
#include "Printer.h"

#include <iostream>

using namespace std;

void VariadicTemplate::print() {
    Printer::templatePrint<>("VariadicTemplate", "print empty", "");
}

template<class T, class... Args>
void VariadicTemplate::print(T head, Args... rest) {
    Printer::templatePrint<T>("VariadicTemplate", "parameter", head);
    print(rest...);
}

void VariadicTemplate::start() {
    print(1, 2, 3, 4);
}