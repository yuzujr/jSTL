#ifndef _JSTL_UTILITY_MOVE_H
#define _JSTL_UTILITY_MOVE_H

#include "type_traits/remove_reference.h"

namespace jstl {

template <typename T>
constexpr remove_reference_t<T>&& move(T&& t) noexcept {
    return static_cast<remove_reference_t<T>&&>(t);
}

}  // namespace jstl

#endif