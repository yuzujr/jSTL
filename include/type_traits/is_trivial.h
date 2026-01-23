#ifndef _JSTL_TYPE_TRAITS_IS_TRIVIAL_H
#define _JSTL_TYPE_TRAITS_IS_TRIVIAL_H

#include "type_traits/integral_constant.h"

namespace jstl {

template <class Tp>
struct is_trivial : integral_constant<bool, __is_trivial(Tp)> {};

template <class Tp>
inline constexpr bool is_trivial_v = is_trivial<Tp>::value;

}  // namespace jstl

#endif
