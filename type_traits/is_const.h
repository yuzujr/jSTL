#ifndef _MY_TYPE_TRAITS_IS_CONST_H
#define _MY_TYPE_TRAITS_IS_CONST_H

#include "type_traits/integral_constant.h"

namespace my {

template <class Tp>
struct is_const : public false_type {};
template <class Tp>
struct is_const<Tp const> : public true_type {};

template <class Tp>
inline constexpr bool is_const_v = is_const<Tp>::value;

}  // namespace my

#endif