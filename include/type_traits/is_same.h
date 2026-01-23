#ifndef _JSTL_TYPE_TRAITS_IS_SAME_H
#define _JSTL_TYPE_TRAITS_IS_SAME_H

#include "type_traits/integral_constant.h"

namespace jstl {

namespace detail {

template <typename T1, typename T2>
struct _is_same {
    const static bool value = false;
};

template <typename T1>
struct _is_same<T1, T1> {
    const static bool value = true;
};

}  // namespace detail

template <class T1, class T2>
struct is_same : bool_constant<detail::_is_same<T1, T2>::value> {};

template <class T1, class T2>
inline constexpr bool is_same_v = detail::_is_same<T1, T2>::value;

// has the same effect as is_same<T1,T2> but instantiates fewer types
template <class T1, class T2>
using _IsSame = bool_constant<detail::_is_same<T1, T2>::value>;

template <class T1, class T2>
using _IsNotSame = bool_constant<!detail::_is_same<T1, T2>::value>;

}  // namespace jstl

#endif