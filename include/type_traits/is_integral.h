#ifndef _JSTL_TYPE_TRAITS_IS_INTEGRAL_H
#define _JSTL_TYPE_TRAITS_IS_INTEGRAL_H

#include "type_traits/integral_constant.h"

namespace jstl {

template <class Tp>
struct is_integral : public bool_constant<__is_integral(Tp)> {};

template <class Tp>
inline constexpr bool is_integral_v = __is_integral(Tp);

}  // namespace jstl

#endif