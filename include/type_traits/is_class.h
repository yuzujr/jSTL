#ifndef _JSTL_TYPE_TRAITS_IS_CLASS_H
#define _JSTL_TYPE_TRAITS_IS_CLASS_H

#include "type_traits/integral_constant.h"
#include "type_traits/is_union.h"

namespace jstl {

namespace detail {
template <class Tp>
integral_constant<bool, !is_union<Tp>::value> test(int Tp::*);

template <class>
false_type test(...);
}  // namespace detail

template <class Tp>
struct is_class : decltype(detail::test<Tp>(nullptr)) {};

template <class Tp>
inline constexpr bool is_class_v = is_class<Tp>::value;

}  // namespace jstl

#endif  // _JSTL_TYPE_TRAITS_IS_CLASS_H