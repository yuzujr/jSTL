#ifndef _JSTL_TYPE_TRAITS_IS_REFERENCEABLE_H
#define _JSTL_TYPE_TRAITS_IS_REFERENCEABLE_H

#include "type_traits/integral_constant.h"
#include "type_traits/is_same.h"

namespace jstl {

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
    : integral_constant<bool, _IsNotSame<decltype(detail::_is_referenceable_impl::_test<Tp>(0)),
                                         false_type>::value> {};
template <class Tp>
inline constexpr bool is_referenceable_v = is_referenceable<Tp>::value;

}  // namespace jstl

#endif