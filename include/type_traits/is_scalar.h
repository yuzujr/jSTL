#ifndef _JSTL_TYPE_TRAITS_IS_SCALAR_H
#define _JSTL_TYPE_TRAITS_IS_SCALAR_H

#include "type_traits/integral_constant.h"
#include "type_traits/is_arithmetic.h"
#include "type_traits/is_enum.h"
#include "type_traits/is_member_pointer.h"
#include "type_traits/is_null_pointer.h"
#include "type_traits/is_pointer.h"

namespace jstl {

template <class Tp>
struct is_scalar
    : integral_constant<bool, is_arithmetic<Tp>::value || is_enum<Tp>::value ||
                                  is_pointer<Tp>::value ||
                                  is_member_pointer<Tp>::value ||
                                  is_null_pointer<Tp>::value> {};

template <class Tp>
inline constexpr bool is_scalar_v = is_scalar<Tp>::value;

}  // namespace jstl

#endif