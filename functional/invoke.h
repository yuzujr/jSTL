#ifndef _MY_FUNCTIONAL_INVOKE_H
#define _MY_FUNCTIONAL_INVOKE_H

#include "type_traits/invoke.h"
#include "utility/forward.h"

namespace my {

template <class Fn, class... Args>
constexpr invoke_result_t<Fn, Args...> invoke(Fn&& fn, Args&&... args) noexcept(
    is_nothrow_invocable_v<Fn, Args...>) {
    return _invoke(forward<Fn>(fn), forward<Args>(args)...);
}

}  // namespace my

#endif