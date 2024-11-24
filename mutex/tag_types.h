#ifndef _MY_MUTEX_TAG_TYPES_H
#define _MY_MUTEX_TAG_TYPES_H

#include <__config>

namespace my {
struct adopt_lock_t {
    explicit adopt_lock_t() = default;
};
}  // namespace my

#endif