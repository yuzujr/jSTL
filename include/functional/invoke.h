#ifndef _JSTL_FUNCTIONAL_INVOKE_H
#define _JSTL_FUNCTIONAL_INVOKE_H

#include "type_traits/invoke.h"
#include "utility/forward.h"

namespace jstl {

template <class Fn, class... Args>
constexpr invoke_result_t<Fn, Args...> invoke(Fn&& fn, Args&&... args) noexcept(
    is_nothrow_invocable_v<Fn, Args...>) {
    return _invoke(jstl::forward<Fn>(fn), jstl::forward<Args>(args)...);
}

}  // namespace jstl

#endif