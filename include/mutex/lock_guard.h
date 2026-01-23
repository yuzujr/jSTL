#ifndef _JSTL_MUTEX_LOCK_GUARD_H
#define _JSTL_MUTEX_LOCK_GUARD_H

#include "mutex/tag_types.h"

namespace jstl {

template <class Mutex>
struct lock_guard {
    typedef Mutex mutex_type;

    explicit lock_guard(Mutex& m) : _m(m) {
        _m.lock();
    }
    lock_guard(Mutex& m, adopt_lock_t) : _m(m) {}
    lock_guard(const lock_guard&) = delete;
    lock_guard& operator=(const lock_guard&) = delete;

    ~lock_guard() {
        _m.unlock();
    }

private:
    mutex_type& _m;
};

}  // namespace jstl

#endif