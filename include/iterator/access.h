#ifndef _JSTL_ITERATOR_ACCESS_H
#define _JSTL_ITERATOR_ACCESS_H

#include <cstddef>

namespace jstl {

// array version
template <class Tp, size_t N>
Tp* begin(Tp (&arr)[N]) {
    return arr;
}
template <class Tp, size_t N>
Tp* end(Tp (&arr)[N]) {
    return arr + N;
}

template <class Cp>
auto begin(Cp& c) -> decltype(c.begin()) {
    return c.begin();
}

template <class Cp>
auto begin(const Cp& c) -> decltype(c.begin()) {
    return c.begin();
}

template <class Cp>
auto end(Cp& c) -> decltype(c.end()) {
    return c.end();
}

template <class Cp>
auto end(const Cp& c) -> decltype(c.end()) {
    return c.end();
}

template <class Cp>
constexpr auto cbegin(const Cp& c) noexcept(noexcept(jstl::begin(c))) -> decltype(jstl::begin(c)) {
    return jstl::begin(c);
}

template <class Cp>
constexpr auto cend(const Cp& c) noexcept(noexcept(jstl::end(c))) -> decltype(jstl::end(c)) {
    return jstl::end(c);
}

}  // namespace jstl

#endif