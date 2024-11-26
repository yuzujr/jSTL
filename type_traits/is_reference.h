#ifndef _MY_TYPE_TRAITS_IS_REFERENCE_H
#define _MY_TYPE_TRAITS_IS_REFERENCE_H

#include "type_traits/integral_constant.h"

namespace my {

namespace detail {

template <class T>
inline constexpr bool _is_lvalue_reference = false;

template <class T>
inline constexpr bool _is_lvalue_reference<T&> = true;

template <class T>
inline constexpr bool _is_rvalue_reference = false;

template <class T>
inline constexpr bool _is_rvalue_reference<T&&> = true;

}  // namespace detail

template <class T>
struct is_lvalue_reference : bool_constant<detail::_is_lvalue_reference<T>> {};

template <class T>
inline constexpr bool is_lvalue_reference_v = detail::_is_lvalue_reference<T>;

template <class T>
struct is_rvalue_reference : bool_constant<detail::_is_rvalue_reference<T>> {};

template <class T>
inline constexpr bool is_rvalue_reference_v = detail::_is_rvalue_reference<T>;
}  // namespace my

#endif