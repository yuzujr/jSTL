#ifndef _MY_MUTEX
#define _MY_MUTEX

#include <mutex>  //cannot do: lock

#include "mutex/lock_guard.h"
#include "mutex/tag_types.h"
#include "tuple.h"

namespace my {

// scoped_lock
template <class... Mutexes>
class scoped_lock {
public:
    explicit scoped_lock(Mutexes &...Mtxes) : My_Mutexes(Mtxes...) {
        std::lock(Mtxes...);
    }

    explicit scoped_lock(my::adopt_lock_t, Mutexes &...Mtxes)
        : My_Mutexes(Mtxes...) {}

    ~scoped_lock() noexcept {
        my::apply(
            [](Mutexes &...Mtxes) {
                (..., Mtxes.unlock());
            },
            My_Mutexes);
    }

    scoped_lock(const scoped_lock &) = delete;
    scoped_lock &operator=(const scoped_lock &) = delete;

private:
    std::tuple<Mutexes &...> My_Mutexes;
};

template <class _Mutex>
class scoped_lock<_Mutex> {
public:
    using mutex_type = _Mutex;
    explicit scoped_lock(_Mutex &_Mtx) : _MyMutex(_Mtx) {
        _MyMutex.lock();
    }

    explicit scoped_lock(my::adopt_lock_t, _Mutex &_Mtx) noexcept
        : _MyMutex(_Mtx) {}

    ~scoped_lock() noexcept {
        _MyMutex.unlock();
    }

    scoped_lock(const scoped_lock &) = delete;
    scoped_lock &operator=(const scoped_lock &) = delete;

private:
    _Mutex &_MyMutex;
};

template <>
class scoped_lock<> {
public:
    explicit scoped_lock() = default;
    explicit scoped_lock(adopt_lock_t) noexcept {}
    scoped_lock(const scoped_lock &) = delete;
    scoped_lock &operator=(const scoped_lock &) = delete;
};

}  // namespace my

#endif