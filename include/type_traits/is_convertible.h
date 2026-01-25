#ifndef _JSTL_TYPE_TRAITS_IS_CONVERTIBLE_H
#define _JSTL_TYPE_TRAITS_IS_CONVERTIBLE_H

#include "type_traits/integral_constant.h"

namespace jstl {

template <class T1, class T2>
struct is_convertible : integral_constant<bool, __is_convertible(T1, T2)> {};

template <class T1, class T2>
inline constexpr bool is_convertible_v = __is_convertible(T1, T2);

}  // namespace jstl

#endif