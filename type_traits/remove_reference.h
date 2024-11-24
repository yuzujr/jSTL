#ifndef _MY_TYPE_TRAITS_REMOVE_REFERENCE_H
#define _MY_TYPE_TRAITS_REMOVE_REFERENCE_H

#include <__config>

namespace my {

template <typename T>
struct remove_reference {
    typedef T type;
};

template <typename T>
struct remove_reference<T&> {
    typedef T type;
};

template <typename T>
struct remove_reference<T&&> {
    typedef T type;
};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

}  // namespace my

#endif