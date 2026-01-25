#ifndef _JSTL_MEMORY_UNINITIALIZED_ALGORITHMS_H
#define _JSTL_MEMORY_UNINITIALIZED_ALGORITHMS_H

#include <cstring>
#include <iterator>

#include "memory/addressof.h"
#include "type_traits/is_array.h"
#include "type_traits/is_trivially_copyable.h"
#include "type_traits/is_trivially_destructible.h"

namespace jstl {

// Copies n elements from the range beginning at first to an uninitialized
// memory area beginning at d_first
template <class InputIt, class Size, class NoThrowForwardIt>
NoThrowForwardIt uninitialized_copy_n(InputIt first, Size n,
                                      NoThrowForwardIt d_first) {
    using T = typename std::iterator_traits<NoThrowForwardIt>::value_type;
    if constexpr (is_trivially_copyable_v<T>) {
        std::memmove(static_cast<void*>(addressof(*d_first)),
                     static_cast<const void*>(addressof(*first)),
                     sizeof(T) * n);
        return d_first + n;
    } else {
        NoThrowForwardIt current = d_first;
        try {
            for (; n > 0; ++first, (void)++current, --n) {
                ::new (static_cast<void*>(addressof(*current))) T(*first);
            }
        } catch (...) {
            for (; d_first != current; ++d_first) {
                d_first->~T();
            }
            throw;
        }
    }
}

// Copies elements from the range [first, last) to an uninitialized memory area
// beginning at d_first
template <class InputIt, class NoThrowForwardIt>
NoThrowForwardIt uninitialized_copy(InputIt first, InputIt last,
                                    NoThrowForwardIt d_first) {
    return uninitialized_copy_n(first, last - first, d_first);
}

// Fills an uninitialized memory area beginning at first with n copies of value
template <class NoThrowForwardIt, class Size, class T>
NoThrowForwardIt uninitialized_fill_n(NoThrowForwardIt first, Size n,
                                      const T& value) {
    using ValueType =
        typename std::iterator_traits<NoThrowForwardIt>::value_type;
    if constexpr (is_trivially_copyable_v<ValueType>) {
        std::memset(static_cast<void*>(addressof(*first)), value,
                    sizeof(ValueType) * n);
        return first + n;
    } else {
        NoThrowForwardIt current = first;
        try {
            for (; n > 0; --n, ++current) {
                ::new (static_cast<void*>(addressof(*current)))
                    ValueType(value);
            }
        } catch (...) {
            for (; first != current; ++first) {
                first->~ValueType();
            }
            throw;
        }
        return current;
    }
}

// Fills an uninitialized memory area beginning at first and ending at last
// with copies of value
template <class NoThrowForwardIt, class T>
void uninitialized_fill(NoThrowForwardIt first, NoThrowForwardIt last,
                        const T& value) {
    uninitialized_fill_n(first, last - first, value);
}

/*
uninitialized_default_construct
(C++17)
        constructs objects by default-initialization in an uninitialized area of
memory, defined by a range (function template) uninitialized_default_construct_n
(C++17)
         (function template)
uninitialized_value_construct
(C++17)
        constructs objects by value-initialization in an uninitialized area of
memory, defined by a range (function template) uninitialized_value_construct_n*/

// moves a number of objects to an uninitialized area of memory
template <class InputIt, class Size, class NoThrowForwardIt>
NoThrowForwardIt uninitialized_move_n(InputIt first, Size n,
                                      NoThrowForwardIt d_first) {
    using T = typename std::iterator_traits<NoThrowForwardIt>::value_type;
    if constexpr (is_trivially_copyable_v<T>) {
        std::memmove(static_cast<void*>(addressof(*d_first)),
                     static_cast<const void*>(addressof(*first)),
                     sizeof(T) * n);
        return d_first + n;
    } else {
        NoThrowForwardIt current = d_first;
        try {
            for (; n > 0; ++first, (void)++current, --n) {
                ::new (static_cast<void*>(addressof(*current)))
                    T(std::move(*first));
            }
        } catch (...) {
            for (; d_first != current; ++d_first) {
                d_first->~T();
            }
            throw;
        }
        return current;
    }
}

// moves a range of objects to an uninitialized area of memory
template <class InputIt, class NoThrowForwardIt>
NoThrowForwardIt uninitialized_move(InputIt first, InputIt last,
                                    NoThrowForwardIt d_first) {
    return uninitialized_move_n(first, last - first, d_first);
}

// constructs objects by default-initialization in an uninitialized area of
// memory, defined by a start and a count
template <class NoThrowForwardIt, class Size>
void uninitialized_default_construct_n(NoThrowForwardIt first, Size n) {
    using ValueType =
        typename std::iterator_traits<NoThrowForwardIt>::value_type;
    NoThrowForwardIt current = first;
    try {
        for (; n > 0; --n, ++current) {
            ::new (static_cast<void*>(addressof(*current))) ValueType;
        }
    } catch (...) {
        for (; first != current; ++first) {
            first->~ValueType();
        }
        throw;
    }
}

// constructs objects by default-initialization in an uninitialized area of
// memory, defined by a range
template <class NoThrowForwardIt>
void uninitialized_default_construct(NoThrowForwardIt first,
                                     NoThrowForwardIt last) {
    uninitialized_default_construct_n(first, last - first);
}

// constructs objects by value-initialization in an uninitialized area of
// memory, defined by a start and a count
template <class NoThrowForwardIt, class Size>
void uninitialized_value_construct_n(NoThrowForwardIt first, Size n) {
    using ValueType =
        typename std::iterator_traits<NoThrowForwardIt>::value_type;
    NoThrowForwardIt current = first;
    try {
        for (; n > 0; --n, ++current) {
            ::new (static_cast<void*>(addressof(*current))) ValueType();
        }
    } catch (...) {
        for (; first != current; ++first) {
            first->~ValueType();
        }
        throw;
    }
}

// constructs objects by value-initialization in an uninitialized area of
// memory, defined by a range
template <class NoThrowForwardIt>
void uninitialized_value_construct(NoThrowForwardIt first,
                                   NoThrowForwardIt last) {
    uninitialized_value_construct_n(first, last - first);
}

// Destroys at
template <class T>
void destroy_at(T* p) {
    if constexpr (is_array_v<T>) {
        for (auto& elem : *p) {
            (destroy_at)(addressof(elem));
        }
    } else {
        p->~T();
    }
}

// Destroys n elements starting from first
template <class NoThrowForwardIt, class Size>
void destroy_n(NoThrowForwardIt first, Size n) {
    using ValueType =
        typename std::iterator_traits<NoThrowForwardIt>::value_type;
    if constexpr (!is_trivially_destructible_v<ValueType>) {
        for (; n > 0; --n, ++first) {
            destroy_at(addressof(*first));
        }
    }
}

// Destroys the elements in the range [first, last)
template <class NoThrowForwardIt>
void destroy(NoThrowForwardIt first, NoThrowForwardIt last) {
    destroy_n(first, last - first);
}

}  // namespace jstl

#endif