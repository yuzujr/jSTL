#ifndef _JSTL_TYPE_TRAITS_IS_FUNCTION_H
#define _JSTL_TYPE_TRAITS_IS_FUNCTION_H

#include "type_traits/integral_constant.h"

namespace jstl {

template <class Tp>
struct is_function : integral_constant<bool, __is_function(Tp)> {};

template <class _Tp>
inline constexpr bool is_function_v = __is_function(_Tp);

}  // namespace jstl

#endif