#ifndef _JSTL_TYPE_TRAITS_IS_REFERENCE_H
#define _JSTL_TYPE_TRAITS_IS_REFERENCE_H

#include "type_traits/integral_constant.h"

namespace jstl {

namespace detail {

template <class Tp>
inline constexpr bool _is_lvalue_reference = false;

template <class Tp>
inline constexpr bool _is_lvalue_reference<Tp&> = true;

template <class Tp>
inline constexpr bool _is_rvalue_reference = false;

template <class Tp>
inline constexpr bool _is_rvalue_reference<Tp&&> = true;

}  // namespace detail

template <class Tp>
struct is_lvalue_reference : bool_constant<detail::_is_lvalue_reference<Tp>> {};

template <class Tp>
inline constexpr bool is_lvalue_reference_v = detail::_is_lvalue_reference<Tp>;

template <class Tp>
struct is_rvalue_reference : bool_constant<detail::_is_rvalue_reference<Tp>> {};

template <class Tp>
inline constexpr bool is_rvalue_reference_v = detail::_is_rvalue_reference<Tp>;

}  // namespace jstl

#endif