#ifndef _JSTL_TYPE_TRAITS_IS_EMPTY_H
#define _JSTL_TYPE_TRAITS_IS_EMPTY_H

#include "type_traits/integral_constant.h"

namespace jstl {

template <class Tp>
struct is_empty : integral_constant<bool, __is_empty(Tp)> {};
template <class Tp>
inline constexpr bool is_empty_v = __is_empty(Tp);

}  // namespace jstl

#endif