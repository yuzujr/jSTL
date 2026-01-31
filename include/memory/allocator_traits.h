#ifndef _JSTL_MEMORY_ALLOCATOR_TRAITS_H
#define _JSTL_MEMORY_ALLOCATOR_TRAITS_H

#include <cstddef>

#include "type_traits/detected_or.h"
#include "type_traits/enable_if.h"
#include "type_traits/integral_constant.h"
#include "type_traits/is_empty.h"
#include "type_traits/remove_reference.h"
#include "utility/declval.h"
#include "utility/forward.h"

namespace jstl {

// __pointer
template <class Tp>
using __pointer_member = typename Tp::pointer;

template <class Tp, class Alloc>
using __pointer = detected_or_t<Tp*, __pointer_member, remove_reference_t<Alloc> >;

// __propagate_on_container_copy_assignment
template <class Tp>
using __propagate_on_container_copy_assignment_member =
    typename Tp::propagate_on_container_copy_assignment;

template <class Alloc>
using __propagate_on_container_copy_assignment =
    detected_or_t<false_type, __propagate_on_container_copy_assignment_member, Alloc>;

// __propagate_on_container_move_assignment
template <class Tp>
using __propagate_on_container_move_assignment_member =
    typename Tp::propagate_on_container_move_assignment;

template <class Alloc>
using __propagate_on_container_move_assignment =
    detected_or_t<false_type, __propagate_on_container_move_assignment_member, Alloc>;

// __propagate_on_container_swap
template <class Tp>
using __propagate_on_container_swap_member = typename Tp::propagate_on_container_swap;

template <class Alloc>
using __propagate_on_container_swap =
    detected_or_t<false_type, __propagate_on_container_swap_member, Alloc>;

// __is_always_equal
template <class Tp>
using __is_always_equal_member = typename Tp::is_always_equal;

template <class Alloc>
using __is_always_equal =
    detected_or_t<typename is_empty<Alloc>::type, __is_always_equal_member, Alloc>;

// __has_construct_v
template <class, class _Alloc, class... _Args>
inline const bool __has_construct_impl = false;

template <class Alloc, class... Args>
inline const bool __has_construct_impl<
    decltype((void)declval<Alloc>().construct(declval<Args>()...)), Alloc, Args...> = true;

template <class Alloc, class... Args>
inline const bool __has_construct_v = __has_construct_impl<void, Alloc, Args...>;

// __has_destroy_v
template <class Alloc, class Pointer, class = void>
inline const bool __has_destroy_v = false;

template <class Alloc, class Pointer>
inline const bool
    __has_destroy_v<Alloc, Pointer, decltype((void)declval<Alloc>().destroy(declval<Pointer>()))> =
        true;

// __has_max_size_v
template <class Alloc, class = void>
inline const bool __has_max_size_v = false;

template <class Alloc>
inline const bool __has_max_size_v<Alloc, decltype((void)declval<Alloc&>().max_size())> = true;

template <class Alloc>
struct allocator_traits {
    using allocator_type = Alloc;
    using value_type = typename Alloc::value_type;
    using pointer = __pointer<value_type, Alloc>;
    using const_pointer = const pointer;
    using void_pointer = void*;
    using const_void_pointer = const void*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_copy_assignment =
        __propagate_on_container_copy_assignment<allocator_type>;
    using propagate_on_container_move_assignment =
        __propagate_on_container_move_assignment<allocator_type>;
    using propagate_on_container_swap = __propagate_on_container_swap<allocator_type>;
    using is_always_equal = __is_always_equal<allocator_type>;

    static pointer allocate(Alloc& a, size_type n) {
        return a.allocate(n);
    }

    static pointer allocate(Alloc& a, size_type n, const_void_pointer hint) {
        return a.allocate(n, hint);
    }

    static void deallocate(Alloc& a, pointer p, size_type n) noexcept {
        a.deallocate(p, n);
    }

    template <class T, class... Args,
              enable_if_t<__has_construct_v<allocator_type, T*, Args...>, int> = 0>
    static void construct(Alloc& a, T* p, Args&&... args) {
        a.construct(p, jstl::forward<Args>(args)...);
    }

    template <class T, class... Args,
              enable_if_t<!__has_construct_v<allocator_type, T*, Args...>, int> = 0>
    static void construct(Alloc& a, T* p, Args&&... args) {
        (void)a;
        ::new ((void*)p) T(jstl::forward<Args>(args)...);
    }

    template <class T, enable_if_t<__has_destroy_v<allocator_type, T*>, int> = 0>
    static void destroy(Alloc& a, T* p) {
        a.destroy(p);
    }

    template <class T, enable_if_t<!__has_destroy_v<allocator_type, T*>, int> = 0>
    static void destroy(Alloc& a, T* p) {
        (void)a;
        p->~T();
    }

    template <class Ap = Alloc, enable_if_t<__has_max_size_v<const Ap>, size_type> = 0>
    static size_type max_size(const Alloc& a) noexcept {
        return a.max_size();
    }

    template <class Ap = Alloc, enable_if_t<!__has_max_size_v<const Ap>, size_type> = 0>
    static size_type max_size(const Alloc&) noexcept {
        return size_type(-1) / sizeof(value_type);
    }

    static Alloc select_on_container_copy_construction(const Alloc& a) {
        return a;
    }
};

}  // namespace jstl

#endif