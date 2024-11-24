#ifndef _MY_TYPE_TRAITS_ADD_LVALUE_REFERENCE_H
#define _MY_TYPE_TRAITS_ADD_LVALUE_REFERENCE_H

#include <__config>

#include "type_traits/is_referenceable.h"

namespace my {

namespace detail {

template <class Tp, bool = is_referenceable<Tp>::value>
struct _add_lvalue_reference_impl {
    typedef Tp type;
};

template <class Tp>
struct _add_lvalue_reference_impl<Tp, true> {
    typedef Tp& type;
};

}  // namespace detail

template <class Tp>
struct add_lvalue_reference {
    using type = typename detail::_add_lvalue_reference_impl<Tp>::type;
};

template <class Tp>
using add_lvalue_reference_t =
    typename detail::_add_lvalue_reference_impl<Tp>::type;

}  // namespace my

#endif