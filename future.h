
#include <future>

#include "type_traits/decay.h"
#include "type_traits/invoke.h"
#include "utility/forward.h"

namespace my {

enum class launch { async = 1, deferred = 2, any = async | deferred };

template <class Fn, class... Args>
[[nodiscard]] std::future<invoke_result_t<decay_t<Fn>, decay_t<Args>...>> async(
    launch policy, Fn&& fn, Args&&... args) {
    using Ret = invoke_result_t<decay_t<Fn>, decay_t<Args>...>;
    using Ptype = int;  // todo...
}

template <class Fn, class... Args>
[[nodiscard]] std::future<invoke_result_t<decay_t<Fn>, decay_t<Args>...>> async(
    Fn&& fn, Args&&... args) {
    return async(launch::async, forward<Fn>(fn), forward<Args>(args)...);
}

}  // namespace my