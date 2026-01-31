#ifndef JSTL_MEMORY_ADDRESOF_H
#define JSTL_MEMORY_ADDRESOF_H

#include "type_traits/enable_if.h"
#include "type_traits/is_object.h"

namespace jstl {

template <class Tp>
typename enable_if<is_object<Tp>::value, Tp*>::type addressof(Tp& arg) noexcept {
    return reinterpret_cast<Tp*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(arg)));
}

template <class Tp>
typename enable_if<!is_object<Tp>::value, Tp*>::type addressof(Tp& arg) noexcept {
    return &arg;
}

}  // namespace jstl

#endif