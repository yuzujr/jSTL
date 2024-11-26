#ifndef _MY_UTILITY_FORWARD_H
#define _MY_UTILITY_FORWARD_H

#include "type_traits/is_reference.h"
#include "type_traits/remove_reference.h"

namespace my {

template <class T>
[[nodiscard]] inline constexpr T&& forward(remove_reference_t<T>& t) noexcept {
    return static_cast<T&&>(t);
}

template <class T>
[[nodiscard]] inline constexpr T&& forward(remove_reference_t<T>&& t) noexcept {
    static_assert(!is_lvalue_reference_v<T>,
                  "cannot forward an rvalue as an lvalue");
    return static_cast<T&&>(t);
}

}  // namespace my

#endif