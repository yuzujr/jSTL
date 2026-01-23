#ifndef _JSTL_MEMORY_ALLOCATOR_H
#define _JSTL_MEMORY_ALLOCATOR_H

#include <cstddef>
#include <new>

#include "memory/addressof.h"
#include "type_traits/integral_constant.h"
#include "type_traits/is_trivially_destructible.h"
#include "utility/forward.h"

namespace jstl {

template <class Tp>
class allocator {
public:
    // member types
    typedef Tp value_type;
    typedef Tp* pointer;
    typedef const Tp* const_pointer;
    typedef Tp& reference;
    typedef const Tp& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef true_type is_always_equal;

    template <class U>
    struct rebind {
        typedef allocator<U> other;
    };

    // member functions
    allocator() noexcept = default;
    allocator(const allocator& other) noexcept {}
    template <class U>
    allocator(const allocator<U>& other) noexcept {}

    ~allocator() = default;

    pointer address(reference x) const noexcept {
        return addressof(x);
    }
    const_pointer address(const_reference x) const noexcept {
        return addressof(x);
    }

    Tp* allocate(std::size_t n, const void* hint) {
        // ignore hint for now
        return allocate(n);
    }
    Tp* allocate(std::size_t n) {
        if (n > max_size()) {
            throw std::bad_alloc();
        }
        if (auto p = static_cast<Tp*>(::operator new(n * sizeof(Tp)))) {
            return p;
        }
        throw std::bad_alloc();
    }

    void deallocate(Tp* p, std::size_t n) {
        ::operator delete(p);
    }

    size_type max_size() const noexcept {
        return size_type(-1) / sizeof(Tp);
    }

    template <class U, class... Args>
    void construct(U* p, Args&&... args) {
        ::new (static_cast<void*>(p)) U(jstl::forward<Args>(args)...);
    }

    template <class U>
    void destroy(U* p) {
        _destroy(p, is_trivially_destructible<U>{});
    }

private:
    // if T is trivially destructible, do nothing
    template <class T>
    void _destroy(T* p, true_type) {}

    // otherwise, call the destructor
    template <class T>
    void _destroy(T* p, false_type) {
        p->~T();
    }
};

template <class T1, class T2>
bool operator==(const allocator<T1>& lhs, const allocator<T2>& rhs) noexcept {
    return true;
}

template <class T1, class T2>
bool operator!=(const allocator<T1>& lhs, const allocator<T2>& rhs) noexcept {
    return false;
}

}  // namespace jstl

#endif