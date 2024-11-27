#ifndef _MY_TYPE_TRAITS_IS_CONSTRUCTIBLE_H
#define _MY_TYPE_TRAITS_IS_CONSTRUCTIBLE_H

#include "type_traits/add_lvalue_reference.h"
#include "type_traits/add_rvalue_reference.h"
#include "type_traits/integral_constant.h"

namespace my {

template <class Tp, class... Args>
struct is_constructible
    : public integral_constant<bool, __is_constructible(Tp, Args...)> {};

template <class Tp, class... Args>
inline constexpr bool is_constructible_v = __is_constructible(Tp, Args...);

template <class Tp>
struct is_copy_constructible
    : public integral_constant<
          bool, __is_constructible(Tp, add_lvalue_reference_t<const Tp>)> {};

template <class Tp>
inline constexpr bool is_copy_constructible_v =
    is_copy_constructible<Tp>::value;

template <class Tp>
struct is_move_constructible
    : public integral_constant<bool, __is_constructible(
                                         Tp, add_rvalue_reference_t<Tp>)> {};

template <class Tp>
inline constexpr bool is_move_constructible_v =
    is_move_constructible<Tp>::value;

template <class Tp>
struct is_default_constructible
    : public integral_constant<bool, __is_constructible(Tp)> {};

template <class Tp>
inline constexpr bool is_default_constructible_v = __is_constructible(Tp);

}  // namespace my

#endif