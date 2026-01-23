#include "thread.h"

namespace jstl {

namespace this_thread {

thread_id_t get_id() noexcept {
#ifdef _WIN32
    return GetCurrentThreadId();
#else
    return pthread_self();
#endif
}

}  // namespace this_thread

thread_id_t thread::get_id() const noexcept {
    return _thr._id;
}

bool operator==(const thread& left, const thread& right) noexcept {
    return left._thr._id == right._thr._id;
}

template <class Ch, class Tr>
auto operator<<(std::basic_ostream<Ch, Tr>& stream, const thread& t) {
    stream << t._thr._id;
    return stream;
}
}  // namespace jstl
