#ifndef _MY_TYPE_TRAITS_REMOVE_CV_H
#define _MY_TYPE_TRAITS_REMOVE_CV_H

#include <__config>

#include "type_traits/remove_const.h"
#include "type_traits/remove_volatile.h"

namespace my {
template <class _Tp>
struct remove_cv {
    typedef remove_volatile_t<remove_const_t<_Tp>> type;
};

template <class _Tp>
using remove_cv_t = remove_volatile_t<remove_const_t<_Tp>>;

}  // namespace my

#endif