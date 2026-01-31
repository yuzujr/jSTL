#ifndef _JSTL_TYPE_TRAITS_IS_NULL_POINTER_H
#define _JSTL_TYPE_TRAITS_IS_NULL_POINTER_H

#include <cstddef>  // for nullptr_t

#include "type_traits/integral_constant.h"
#include "type_traits/is_same.h"
#include "type_traits/remove_cv.h"

namespace jstl {

template <class Tp>
struct is_null_pointer : integral_constant<bool, is_same_v<__remove_cv(Tp), std::nullptr_t>> {};
template <class Tp>
inline constexpr bool is_null_pointer_v = is_same_v<__remove_cv(Tp), std::nullptr_t>;

}  // namespace jstl

#endif