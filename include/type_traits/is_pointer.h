#ifndef _JSTL_TYPE_TRAITS_IS_POINTER_H
#define _JSTL_TYPE_TRAITS_IS_POINTER_H

#include "type_traits/integral_constant.h"

namespace jstl {

template <class Tp>
struct is_pointer : integral_constant<bool, __is_pointer(Tp)> {};
template <class Tp>
inline constexpr bool is_pointer_v = __is_pointer(Tp);

}  // namespace jstl

#endif