#ifndef _MY_UTILITY_SWAP_H
#define _MY_UTILITY_SWAP_H

#include "type_traits/is_assignable.h"
#include "type_traits/is_constructible.h"
#include "type_traits/is_nothrow_assignable.h"
#include "type_traits/is_nothrow_constructible.h"
#include "type_traits/is_swappable.h"
#include "utility/declval.h"
#include "utility/move.h"

namespace my {

template <class Tp>
inline constexpr _swap_result_t<Tp> swap(Tp& x, Tp& y) noexcept(
    is_nothrow_move_constructible<Tp>::value &&
    is_nothrow_move_assignable<Tp>::value) {
    Tp t(move(x));
    x = move(y);
    y = move(t);
}

template <class Tp, size_t Np, enable_if_t<_is_swappable_v<Tp>, int>>
inline constexpr void swap(Tp (&a)[Np],
                           Tp (&b)[Np]) noexcept(_is_nothrow_swappable_v<Tp>) {
    for (size_t i = 0; i != Np; ++i) {
        swap(a[i], b[i]);
    }
}

}  // namespace my

#endif