#ifndef _JSTL_UTILITY_DECLVAL_H
#define _JSTL_UTILITY_DECLVAL_H

#include "type_traits/add_rvalue_reference.h"

namespace jstl {

template <class Tp>
typename add_rvalue_reference<Tp>::type declval() noexcept {
    static_assert(false, "declval not allowed in an evaluated context");
}

}  // namespace jstl

#endif