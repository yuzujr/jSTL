#ifndef _JSTL_TYPE_TRAITS_IS_UNION_H
#define _JSTL_TYPE_TRAITS_IS_UNION_H

#include "type_traits/integral_constant.h"

namespace jstl {

template <class Tp>
struct is_union : integral_constant<bool, __is_union(Tp)> {};

template <class Tp>
inline constexpr bool is_union_v = __is_union(Tp);

}  // namespace jstl

#endif