#ifndef _JSTL_TYPE_TRAITS_IS_TRIVIALLY_COPYABLE_H
#define _JSTL_TYPE_TRAITS_IS_TRIVIALLY_COPYABLE_H

#include "type_traits/integral_constant.h"

namespace jstl {

template <class Tp>
struct is_trivially_copyable : integral_constant<bool, __is_trivially_copyable(Tp)> {};

template <class Tp>
inline constexpr bool is_trivially_copyable_v = is_trivially_copyable<Tp>::value;

}  // namespace jstl

#endif
