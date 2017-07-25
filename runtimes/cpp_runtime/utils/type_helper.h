#ifndef TYPE_HELPER_H
#define TYPE_HELPER_H

#include <iostream>
#include <typeinfo>
#include <cxxabi.h>

inline
void print_type(const std::type_info& info) {
    int s;
    std::cout << abi::__cxa_demangle(info.name(), 0, 0, &s) << std::endl;
}

template<typename T>
void print_type() {
    print_type(typeid(T));
}

#endif