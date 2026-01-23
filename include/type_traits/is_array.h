#ifndef _JSTL_TYPE_TRAITS_IS_ARRAY_H
#define _JSTL_TYPE_TRAITS_IS_ARRAY_H

#include <cstddef>

#include "type_traits/integral_constant.h"

namespace jstl {

template <class Tp>
struct is_array : public false_type {};
template <class Tp>
struct is_array<Tp[]> : public true_type {};
template <class Tp, size_t Np>
struct is_array<Tp[Np]> : public true_type {};

template <class Tp>
inline constexpr bool is_array_v = is_array<Tp>::value;

}  // namespace jstl

#endif