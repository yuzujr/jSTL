#ifndef _MY_TYPE_TRAITS_IS_ARRAY_H
#define _MY_TYPE_TRAITS_IS_ARRAY_H

#include "type_traits/integral_constant.h"

namespace my {
template <class Tp>
struct is_array : public false_type {};
template <class Tp>
struct is_array<Tp[]> : public true_type {};
template <class Tp, size_t Np>
struct is_array<Tp[Np]> : public true_type {};

template <class Tp>
inline constexpr bool is_array_v = is_array<Tp>::value;
}  // namespace my

#endif