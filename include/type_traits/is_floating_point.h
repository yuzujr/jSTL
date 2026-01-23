#ifndef _JSTL_TYPE_TRAITS_IS_FLOATING_POINT_H
#define _JSTL_TYPE_TRAITS_IS_FLOATING_POINT_H

#include "type_traits/integral_constant.h"
#include "type_traits/is_same.h"
#include "type_traits/remove_cv.h"

namespace jstl {

template <class Tp>
struct is_floating_point
    : integral_constant<
          bool, is_same<float, typename remove_cv<Tp>::type>::value ||
                    is_same<double, typename remove_cv<Tp>::type>::value ||
                    is_same<long double, typename remove_cv<Tp>::type>::value> {
};

template <class Tp>
inline constexpr bool is_floating_point_v = is_floating_point<Tp>::value;

}  // namespace jstl

#endif