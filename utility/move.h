#ifndef _MY_UTILITY_MOVE_H
#define _MY_UTILITY_MOVE_H

#include <__config>

#include "type_traits/remove_reference.h"

namespace my {

template <typename T>
remove_reference_t<T>&& move(T t) {
    return static_cast<remove_reference_t<T>&&>(t);
}

}  // namespace my

#endif