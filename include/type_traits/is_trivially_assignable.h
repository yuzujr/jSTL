#ifndef _JSTL_TYPE_TRAITS_IS_TRIVIALLY_ASSIGNABLE_H
#define _JSTL_TYPE_TRAITS_IS_TRIVIALLY_ASSIGNABLE_H

#include "type_traits/add_lvalue_reference.h"
#include "type_traits/add_rvalue_reference.h"
#include "type_traits/integral_constant.h"

namespace jstl {

template <class Tp, class Arg>
struct is_trivially_assignable
    : integral_constant<bool, __is_trivially_assignable(Tp, Arg)> {};

template <class Tp, class Arg>
inline constexpr bool is_trivially_assignable_v =
    __is_trivially_assignable(Tp, Arg);

template <class Tp>
struct is_trivially_copy_assignable
    : integral_constant<bool, __is_trivially_assignable(
                                  add_lvalue_reference_t<Tp>,
                                  add_lvalue_reference_t<const Tp>)> {};

template <class Tp>
inline constexpr bool is_trivially_copy_assignable_v =
    is_trivially_copy_assignable<Tp>::value;

template <class Tp>
struct is_trivially_move_assignable
    : integral_constant<bool, __is_trivially_assignable(
                                  add_lvalue_reference_t<Tp>,
                                  add_rvalue_reference_t<Tp>)> {};

template <class Tp>
inline constexpr bool is_trivially_move_assignable_v =
    is_trivially_move_assignable<Tp>::value;

}  // namespace jstl

#endif
