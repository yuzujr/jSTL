#ifndef _JSTL_TYPE_TRAITS_IS_TRIVIALLY_DESTRUCTIBLE_H
#define _JSTL_TYPE_TRAITS_IS_TRIVIALLY_DESTRUCTIBLE_H

#include "type_traits/integral_constant.h"

namespace jstl {

template <class Tp>
struct is_trivially_destructible
    : integral_constant<bool, __is_trivially_destructible(Tp)> {};

template <class Tp>
inline constexpr bool is_trivially_destructible_v =
    is_trivially_destructible<Tp>::value;

}  // namespace jstl

#endif