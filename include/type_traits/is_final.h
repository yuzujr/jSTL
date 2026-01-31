#ifndef _JSTL_TYPE_TRAITS_IS_FINAL_H
#define _JSTL_TYPE_TRAITS_IS_FINAL_H

#include "type_traits/integral_constant.h"

namespace jstl {

template <typename Tp>
struct is_final : integral_constant<bool, __is_final(Tp)> {};

template <typename Tp>
inline constexpr bool is_final_v = __is_final(Tp);

}  // namespace jstl

#endif