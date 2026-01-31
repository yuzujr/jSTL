#ifndef _JSTL_UTILITY_EXCHANGE_H
#define _JSTL_UTILITY_EXCHANGE_H

#include "type_traits/is_nothrow_assignable.h"
#include "type_traits/is_nothrow_constructible.h"
#include "utility/forward.h"
#include "utility/move.h"

namespace jstl {

template <class T1, class T2 = T1>
inline constexpr T1 exchange(T1& obj,
                             T2&& new_value) noexcept(is_nothrow_move_constructible<T1>::value &&
                                                      is_nothrow_assignable<T1&, T2>::value) {
    T1 old_value = move(obj);
    obj = forward<T2>(new_value);
    return old_value;
}

}  // namespace jstl

#endif