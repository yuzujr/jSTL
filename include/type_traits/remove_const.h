#ifndef _JSTL_TYPE_TRAITS_REMOVE_CONST_H
#define _JSTL_TYPE_TRAITS_REMOVE_CONST_H

namespace jstl {

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

}  // namespace jstl

#endif