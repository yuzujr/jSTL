#ifndef _JSTL_TYPE_TRAITS_UNDERLYING_TYPE_H
#define _JSTL_TYPE_TRAITS_UNDERLYING_TYPE_H

#include "type_traits/is_enum.h"

namespace jstl {

template <class Tp, bool>
struct __underlying_type_impl;

template <class Tp>
struct __underlying_type_impl<Tp, false> {};

template <class Tp>
struct __underlying_type_impl<Tp, true> {
    typedef __underlying_type(Tp) type;
};

template <class Tp>
struct underlying_type : __underlying_type_impl<Tp, is_enum<Tp>::value> {};

// GCC doesn't SFINAE away when using __underlying_type directly
// note: Let GCC go to __underlying_type_impl<Tp, false> when Tp is not enum,
// which avoid compiling error.
template <class Tp>
using underlying_type_t = typename underlying_type<Tp>::type;

}  // namespace jstl

#endif