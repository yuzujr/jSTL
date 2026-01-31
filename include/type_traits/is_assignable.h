#ifndef _JSTL_TYPE_TRAITS_IS_ASSIGNABLE_H
#define _JSTL_TYPE_TRAITS_IS_ASSIGNABLE_H

#include "type_traits/add_lvalue_reference.h"
#include "type_traits/add_rvalue_reference.h"
#include "type_traits/integral_constant.h"

namespace jstl {

template <class Tp, class Up>
struct is_assignable : bool_constant<__is_assignable(Tp, Up)> {};

template <class _Tp, class _Arg>
inline constexpr bool is_assignable_v = __is_assignable(_Tp, _Arg);

template <class _Tp>
struct is_copy_assignable
    : public integral_constant<bool, __is_assignable(add_lvalue_reference_t<_Tp>,
                                                     add_lvalue_reference_t<const _Tp>)> {};

template <class _Tp>
inline constexpr bool is_copy_assignable_v = is_copy_assignable<_Tp>::value;

template <class _Tp>
struct is_move_assignable
    : public integral_constant<bool, __is_assignable(add_lvalue_reference_t<_Tp>,
                                                     add_rvalue_reference_t<_Tp>)> {};

template <class _Tp>
inline constexpr bool is_move_assignable_v = is_move_assignable<_Tp>::value;

}  // namespace jstl

#endif