#ifndef _JSTL_MUTEX_TAG_TYPES_H
#define _JSTL_MUTEX_TAG_TYPES_H

namespace jstl {
struct adopt_lock_t {
    explicit adopt_lock_t() = default;
};
}  // namespace jstl

#endif