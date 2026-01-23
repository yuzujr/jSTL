#ifndef _JSTL_TYPE_TRAITS_ENABLE_IF_H
#define _JSTL_TYPE_TRAITS_ENABLE_IF_H

namespace jstl {

template <bool, class Tp = void>
struct enable_if {};
template <class Tp>
struct enable_if<true, Tp> {
    typedef Tp type;
};

template <bool Bp, class Tp = void>
using enable_if_t = typename enable_if<Bp, Tp>::type;

}  // namespace jstl

#endif