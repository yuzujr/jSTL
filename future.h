#include <__config>

namespace my {

enum class launch { async = 1, deferred = 2, any = async | deferred };

template <class Fn, class... Args>
[[nodiscard]] auto async(launch policy, Fn&& fn, Args&&... args) {}

template <class Fn, class... Args>
[[nodiscard]] auto async(Fn&& fn, Args&&... args) {
    return async(launch::async, forward<Fn>(fn), forward<Args>(args)...);
}

}  // namespace my