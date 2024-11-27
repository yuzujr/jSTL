#ifndef _MY_TYPE_TRAITS_REMOVE_CVREF_H
#define _MY_TYPE_TRAITS_REMOVE_CVREF_H

#include "type_traits/is_same.h"
#include "type_traits/remove_cv.h"
#include "type_traits/remove_reference.h"

namespace my {

template <class Tp>
struct remove_cvref {
    using type = remove_cv_t<remove_reference_t<Tp>>;
};

template <class Tp>
using remove_cvref_t = typename remove_cvref<Tp>::type;

template <class Tp, class Up>
struct __is_same_uncvref : is_same<remove_cvref_t<Tp>, remove_cvref_t<Up>> {};

}  // namespace my

#endif