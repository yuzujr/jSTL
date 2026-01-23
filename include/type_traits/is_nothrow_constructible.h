#ifndef _JSTL_TYPE_TRAITS_IS_NOTHROW_CONSTRUCTIBLE_H
#define _JSTL_TYPE_TRAITS_IS_NOTHROW_CONSTRUCTIBLE_H

#include "type_traits/add_lvalue_reference.h"
#include "type_traits/add_rvalue_reference.h"
#include "type_traits/integral_constant.h"

namespace jstl {

template <class Tp, class... _Args>
struct is_nothrow_constructible
    : public integral_constant<bool, __is_nothrow_constructible(Tp, _Args...)> {
};

template <class Tp, class... _Args>
inline constexpr bool is_nothrow_constructible_v =
    is_nothrow_constructible<Tp, _Args...>::value;

template <class Tp>
struct is_nothrow_copy_constructible
    : public integral_constant<bool,
                               __is_nothrow_constructible(
                                   Tp, add_lvalue_reference_t<const Tp>)> {};

template <class Tp>
inline constexpr bool is_nothrow_copy_constructible_v =
    is_nothrow_copy_constructible<Tp>::value;

template <class Tp>
struct is_nothrow_move_constructible
    : public integral_constant<bool, __is_nothrow_constructible(
                                         Tp, add_rvalue_reference_t<Tp>)> {};

template <class Tp>
inline constexpr bool is_nothrow_move_constructible_v =
    is_nothrow_move_constructible<Tp>::value;

template <class Tp>
struct is_nothrow_default_constructible
    : public integral_constant<bool, __is_nothrow_constructible(Tp)> {};

template <class Tp>
inline constexpr bool is_nothrow_default_constructible_v =
    is_nothrow_default_constructible<Tp>::value;

}  // namespace jstl

#endif