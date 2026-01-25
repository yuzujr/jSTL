#ifndef _JSTL_TYPE_TRAITS_IS_ARITHMETIC_H
#define _JSTL_TYPE_TRAITS_IS_ARITHMETIC_H

#include "type_traits/integral_constant.h"
#include "type_traits/is_floating_point.h"
#include "type_traits/is_integral.h"

namespace jstl {

template <class Tp>
struct is_arithmetic
    : integral_constant<bool, is_integral<Tp>::value ||
                                  is_floating_point<Tp>::value> {};

template <class Tp>
inline constexpr bool is_arithmetic_v = is_arithmetic<Tp>::value;

}  // namespace jstl

#endif