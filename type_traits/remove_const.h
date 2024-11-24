#ifndef _MY_TYPE_TRAITS_REMOVE_CONST_H
#define _MY_TYPE_TRAITS_REMOVE_CONST_H

#include <__config>

namespace my {

template <class Tp>
struct remove_const {
    typedef Tp type;
};
template <class Tp>
struct remove_const<const Tp> {
    typedef Tp type;
};

template <class Tp>
using remove_const_t = typename remove_const<Tp>::type;

}  // namespace my

#endif