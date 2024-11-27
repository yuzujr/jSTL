#ifndef _MY_UTILITY_FORWARD_LIKE_H
#define _MY_UTILITY_FORWARD_LIKE_H

#include "type_traits/conditional.h"
#include "type_traits/is_const.h"
#include "type_traits/is_reference.h"
#include "type_traits/remove_reference.h"

namespace my {
template <class Ap, class Bp>
using _CopyConst = _If<is_const_v<Ap>, const Bp, Bp>;

template <class Ap, class Bp>
using _OverrideRef =
    _If<is_rvalue_reference_v<Ap>, remove_reference_t<Bp>&&, Bp&>;

template <class Ap, class Bp>
using _ForwardLike =
    _OverrideRef<Ap&&,
                 _CopyConst<remove_reference_t<Ap>, remove_reference_t<Bp>>>;

template <class Tp, class Up>
[[nodiscard]] constexpr auto forward_like(Up&& ux) noexcept
    -> _ForwardLike<Tp, Up> {
    return static_cast<_ForwardLike<Tp, Up>>(ux);
}

}  // namespace my

#endif