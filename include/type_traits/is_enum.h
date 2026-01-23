#ifndef _JSTL_TYPE_TRAITS_IS_ENUM_H
#define _JSTL_TYPE_TRAITS_IS_ENUM_H

#include "type_traits/integral_constant.h"

namespace jstl {

template <class Tp>
struct is_enum : integral_constant<bool, __is_enum(Tp)> {};
template <class Tp>
inline constexpr bool is_enum_v = __is_enum(Tp);

}  // namespace jstl

#endif