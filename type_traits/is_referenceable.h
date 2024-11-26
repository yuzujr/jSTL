#ifndef _MY_TYPE_TRAITS_IS_REFERENCEABLE_H
#define _MY_TYPE_TRAITS_IS_REFERENCEABLE_H

#include "type_traits/integral_constant.h"
#include "type_traits/is_same.h"

namespace my {

namespace detail {

struct _is_referenceable_impl {
    template <class Tp>
    static Tp& _test(int);
    template <class Tp>
    static false_type _test(...);
};

}  // namespace detail

template <class Tp>
struct is_referenceable
    : integral_constant<
          bool,
          _IsNotSame<decltype(detail::_is_referenceable_impl::_test<Tp>(0)),
                     false_type>::value> {};

}  // namespace my

#endif