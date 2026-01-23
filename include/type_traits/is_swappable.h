#ifndef _JSTL_TYPE_TRAITS_IS_SWAPPABLE_H
#define _JSTL_TYPE_TRAITS_IS_SWAPPABLE_H

#include <cstddef>

#include "type_traits/add_lvalue_reference.h"
#include "type_traits/enable_if.h"
#include "type_traits/is_assignable.h"
#include "type_traits/is_constructible.h"
#include "type_traits/is_nothrow_assignable.h"
#include "type_traits/is_nothrow_constructible.h"
#include "type_traits/void_t.h"
#include "utility/declval.h"

namespace jstl {

template <class Tp, class Up, class = void>
inline const bool _is_swappable_with_v = false;

template <class Tp>
inline const bool _is_swappable_v = _is_swappable_with_v<Tp&, Tp&>;

template <class Tp, class Up, bool = _is_swappable_with_v<Tp, Up>>
inline const bool _is_nothrow_swappable_with_v = false;

template <class Tp>
inline const bool _is_nothrow_swappable_v =
    _is_nothrow_swappable_with_v<Tp&, Tp&>;

template <class Tp>
using _swap_result_t = enable_if_t<is_move_constructible<Tp>::value &&
                                   is_move_assignable<Tp>::value>;

template <class Tp>
inline constexpr _swap_result_t<Tp> swap(Tp& x, Tp& y) noexcept(
    is_nothrow_move_constructible<Tp>::value &&
    is_nothrow_move_assignable<Tp>::value);

template <class Tp, size_t Np, enable_if_t<_is_swappable_v<Tp>, int> = 0>
inline constexpr void swap(Tp (&a)[Np],
                           Tp (&b)[Np]) noexcept(_is_nothrow_swappable_v<Tp>);

/*ALL generic swap overloads MUST already have a declaration
 available at this point.*/

template <class Tp, class Up>
inline const bool
    _is_swappable_with_v<Tp, Up,
                         void_t<decltype(swap(declval<Tp>(), declval<Up>())),
                                decltype(swap(declval<Up>(), declval<Tp>()))>> =
        true;

template <class Tp, class Up>
inline const bool _is_nothrow_swappable_with_v<Tp, Up, true> =
    noexcept(swap(declval<Tp>(), declval<Up>())) &&
    noexcept(swap(declval<Up>(), declval<Tp>()));

template <class Tp, class Up>
inline constexpr bool is_swappable_with_v = _is_swappable_with_v<Tp, Up>;

template <class Tp, class Up>
struct is_swappable_with : bool_constant<is_swappable_with_v<Tp, Up>> {};

template <class Tp>
inline constexpr bool is_swappable_v =
    is_swappable_with_v<add_lvalue_reference_t<Tp>, add_lvalue_reference_t<Tp>>;

template <class Tp>
struct is_swappable : bool_constant<is_swappable_v<Tp>> {};

template <class Tp, class Up>
inline constexpr bool is_nothrow_swappable_with_v =
    _is_nothrow_swappable_with_v<Tp, Up>;

template <class Tp, class Up>
struct is_nothrow_swappable_with
    : bool_constant<is_nothrow_swappable_with_v<Tp, Up>> {};

template <class Tp>
inline constexpr bool is_nothrow_swappable_v =
    is_nothrow_swappable_with_v<add_lvalue_reference_t<Tp>,
                                add_lvalue_reference_t<Tp>>;

template <class Tp>
struct is_nothrow_swappable : bool_constant<is_nothrow_swappable_v<Tp>> {};

}  // namespace jstl

#endif