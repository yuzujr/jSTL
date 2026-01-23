#ifndef _JSTL_TYPE_TRAITS_IS_OBJECT_H
#define _JSTL_TYPE_TRAITS_IS_OBJECT_H

#include "type_traits/integral_constant.h"
#include "type_traits/is_array.h"
#include "type_traits/is_class.h"
#include "type_traits/is_scalar.h"
#include "type_traits/is_union.h"

namespace jstl {

template <class Tp>
struct is_object
    : integral_constant<bool, is_scalar<Tp>::value || is_array<Tp>::value ||
                                  is_union<Tp>::value || is_class<Tp>::value> {
};

template <class Tp>
inline constexpr bool is_object_v = is_object<Tp>::value;

}  // namespace jstl

#endif