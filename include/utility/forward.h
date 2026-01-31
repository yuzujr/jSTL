#ifndef _JSTL_UTILITY_FORWARD_H
#define _JSTL_UTILITY_FORWARD_H

#include "type_traits/is_reference.h"
#include "type_traits/remove_reference.h"

namespace jstl {

template <class Tp>
[[nodiscard]] inline constexpr Tp&& forward(remove_reference_t<Tp>& t) noexcept {
    return static_cast<Tp&&>(t);
}

template <class Tp>
[[nodiscard]] inline constexpr Tp&& forward(remove_reference_t<Tp>&& t) noexcept {
    static_assert(!is_lvalue_reference_v<Tp>, "cannot forward an rvalue as an lvalue");
    return static_cast<Tp&&>(t);
}

}  // namespace jstl

#endif