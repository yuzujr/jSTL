#ifndef _MY_TYPE_TRAITS_ADD_POINTER_H
#define _MY_TYPE_TRAITS_ADD_POINTER_H

#include "type_traits/is_referenceable.h"
#include "type_traits/is_void.h"
#include "type_traits/remove_reference.h"

namespace my {

namespace detail {

template <class Tp, bool = is_referenceable<Tp>::value || is_void<Tp>::value>
struct add_pointer_impl {
    typedef remove_reference_t<Tp>* type;
};
template <class Tp>
struct add_pointer_impl<Tp, false> {
    typedef Tp type;
};

}  // namespace detail

template <class Tp>
struct add_pointer {
    using type = typename detail::add_pointer_impl<Tp>::type;
};

template <class Tp>
using add_pointer_t = typename detail::add_pointer_impl<Tp>::type;

}  // namespace my

#endif