#ifndef _JSTL_TYPE_TRAITS_IS_TRIVIALLY_CONSTRUCTIBLE_H
#define _JSTL_TYPE_TRAITS_IS_TRIVIALLY_CONSTRUCTIBLE_H

#include "type_traits/add_lvalue_reference.h"
#include "type_traits/add_rvalue_reference.h"
#include "type_traits/integral_constant.h"

namespace jstl {

template <class Tp, class... Args>
struct is_trivially_constructible
    : integral_constant<bool, __is_trivially_constructible(Tp, Args...)> {};

template <class Tp, class... Args>
inline constexpr bool is_trivially_constructible_v = __is_trivially_constructible(Tp, Args...);

template <class Tp>
struct is_trivially_copy_constructible
    : integral_constant<bool, __is_trivially_constructible(Tp, add_lvalue_reference_t<const Tp>)> {
};

template <class Tp>
inline constexpr bool is_trivially_copy_constructible_v =
    is_trivially_copy_constructible<Tp>::value;

template <class Tp>
struct is_trivially_move_constructible
    : integral_constant<bool, __is_trivially_constructible(Tp, add_rvalue_reference_t<Tp>)> {};

template <class Tp>
inline constexpr bool is_trivially_move_constructible_v =
    is_trivially_move_constructible<Tp>::value;

template <class Tp>
struct is_trivially_default_constructible
    : integral_constant<bool, __is_trivially_constructible(Tp)> {};

template <class Tp>
inline constexpr bool is_trivially_default_constructible_v =
    is_trivially_default_constructible<Tp>::value;

}  // namespace jstl

#endif
