#ifndef _MY_TYPE_TRAITS_REMOVE_VOLATILE_H
#define _MY_TYPE_TRAITS_REMOVE_VOLATILE_H

namespace my {

template <class Tp>
struct remove_volatile {
    typedef Tp type;
};
template <class Tp>
struct remove_volatile<volatile Tp> {
    typedef Tp type;
};

template <class Tp>
using remove_volatile_t = typename remove_volatile<Tp>::type;

}  // namespace my

#endif