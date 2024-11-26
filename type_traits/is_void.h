#ifndef _MY_TYPE_TRAITS_IS_VOID_H
#define _MY_TYPE_TRAITS_IS_VOID_H

#include "type_traits/integral_constant.h"
#include "type_traits/is_same.h"
#include "type_traits/remove_cv.h"

namespace my {

template <class Tp>
struct is_void
    : bool_constant<is_same<typename remove_cv<Tp>::type, void>::value> {};

template <class Tp>
inline constexpr bool is_void_v =
    is_same<typename remove_cv<Tp>::type, void>::value;

}  // namespace my

#endif