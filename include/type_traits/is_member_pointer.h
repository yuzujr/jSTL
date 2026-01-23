#ifndef _JSTL_TYPE_TRAITS_IS_MEMBER_POINTER_H
#define _JSTL_TYPE_TRAITS_IS_MEMBER_POINTER_H

#include "type_traits/integral_constant.h"
#include "type_traits/remove_cv.h"

namespace jstl {

template <class T>
struct is_member_pointer_helper : false_type {};

template <class T, class U>
struct is_member_pointer_helper<T U::*> : true_type {};

template <class T>
struct is_member_pointer
    : is_member_pointer_helper<typename remove_cv<T>::type> {};

template <class T>
inline constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

}  // namespace jstl

#endif