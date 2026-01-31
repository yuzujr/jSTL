#ifndef _JSTL_VECTOR_VECTOR_H
#define _JSTL_VECTOR_VECTOR_H

#include <initializer_list>

#include "iterator/reverse_iterator.h"
#include "memory/allocator.h"
#include "memory/allocator_traits.h"
#include "memory/compressed_pair.h"
#include "memory/uninitialized_algorithms.h"
#include "type_traits/is_nothrow_constructible.h"
#include "type_traits/is_trivially_constructible.h"
#include "utility/move.h"

namespace jstl {

template <class T, class Allocator = allocator<T> >
class vector {
public:
    using value_type = T;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = typename allocator_traits<allocator_type>::size_type;
    using difference_type = typename allocator_traits<allocator_type>::difference_type;
    using pointer = typename allocator_traits<allocator_type>::pointer;
    using const_pointer = typename allocator_traits<allocator_type>::const_pointer;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = jstl::reverse_iterator<iterator>;
    using const_reverse_iterator = jstl::reverse_iterator<const_iterator>;

    vector() noexcept(is_nothrow_default_constructible<allocator_type>::value)
        : __cap_and_alloc_{nullptr, allocator_type()} {}
    explicit vector(const allocator_type& __a) noexcept : __cap_and_alloc_{nullptr, __a} {}
    explicit vector(size_type n) : __cap_and_alloc_{nullptr, allocator_type()} {
        init_n(n);
    }
    explicit vector(size_type n, const allocator_type& __a) : __cap_and_alloc_{nullptr, __a} {
        init_n(n);
    }
    vector(size_type n, const value_type& value, const allocator_type& __a = allocator_type())
        : __cap_and_alloc_{nullptr, __a} {
        init_n(n, value);
    }
    template <class InputIterator>
    vector(InputIterator first, InputIterator last, const allocator_type& __a = allocator_type())
        : __cap_and_alloc_{nullptr, __a} {
        for (; first != last; ++first) {
            push_back(*first);
        }
    }
    vector(const vector& x)
        : __cap_and_alloc_{
              nullptr,
              allocator_traits<allocator_type>::select_on_container_copy_construction(x.alloc())} {
        reserve(x.size());
        jstl::uninitialized_copy(x.__begin_, x.__end_, __begin_, alloc());
        __end_ = __begin_ + x.size();
    }
    vector(vector&& x) noexcept(is_nothrow_move_constructible<allocator_type>::value)
        : __begin_(x.__begin_), __end_(x.__end_), __cap_and_alloc_(jstl::move(x.__cap_and_alloc_)) {
        x.__begin_ = nullptr;
        x.__end_ = nullptr;
        x.__cap_and_alloc_.first() = nullptr;
    }
    vector(std::initializer_list<value_type> il) : vector(il.begin(), il.end(), allocator_type()) {}
    vector(std::initializer_list<value_type> il, const allocator_type& a)
        : vector(il.begin(), il.end(), a) {}

    ~vector() {
        if (__begin_) {
            jstl::destroy(__begin_, __end_);
            alloc().deallocate(__begin_, capacity());
        }
    }

    vector& operator=(const vector& x);
    vector& operator=(vector&& x) noexcept(
        allocator_type::propagate_on_container_move_assignment::value ||
        allocator_type::is_always_equal::value);  // C++17
    vector& operator=(std::initializer_list<value_type> il);

    template <class InputIterator>
    void assign(InputIterator first, InputIterator last);
    void assign(size_type n, const value_type& u);
    void assign(std::initializer_list<value_type> il);

    allocator_type get_allocator() const noexcept {
        return alloc();
    }

    iterator begin() noexcept {
        return __begin_;
    }
    const_iterator begin() const noexcept {
        return __begin_;
    }
    iterator end() noexcept {
        return __end_;
    }
    const_iterator end() const noexcept {
        return __end_;
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }
    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }
    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }
    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    const_iterator cbegin() const noexcept {
        return __begin_;
    }
    const_iterator cend() const noexcept {
        return __end_;
    }
    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(end());
    }
    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(begin());
    }

    size_type size() const noexcept {
        return static_cast<size_type>(__end_ - __begin_);
    }
    size_type max_size() const noexcept {
        return allocator_traits<allocator_type>::max_size(alloc());
    }
    size_type capacity() const noexcept {
        return static_cast<size_type>(cap() - __begin_);
    }
    bool empty() const noexcept {
        return __begin_ == __end_;
    }
    void reserve(size_type n);
    void shrink_to_fit() noexcept;

    reference operator[](size_type n) {
        return *(__begin_ + n);
    }
    const_reference operator[](size_type n) const {
        return *(__begin_ + n);
    }
    reference at(size_type n) {
        if (n >= size()) {
            throw std::out_of_range("vector::at: index out of range");
        }
        return (*this)[n];
    }
    const_reference at(size_type n) const {
        if (n >= size()) {
            throw std::out_of_range("vector::at: index out of range");
        }
        return (*this)[n];
    }

    reference front() {
        return *__begin_;
    }
    const_reference front() const {
        return *__begin_;
    }
    reference back() {
        return *(__end_ - 1);
    }
    const_reference back() const {
        return *(__end_ - 1);
    }

    value_type* data() noexcept {
        return static_cast<value_type*>(static_cast<void*>(__begin_));
    }
    const value_type* data() const noexcept {
        return static_cast<const value_type*>(static_cast<const void*>(__begin_));
    }

    void push_back(const value_type& x) {
        if (__end_ == cap()) {
            reserve(size() + 1);
        }
        allocator_traits<allocator_type>::construct(alloc(), __end_, x);
        ++__end_;
    }
    void push_back(value_type&& x) {
        if (__end_ == cap()) {
            reserve(size() + 1);
        }
        allocator_traits<allocator_type>::construct(alloc(), __end_, jstl::move(x));
        ++__end_;
    }
    template <class... Args>
    reference emplace_back(Args&&... args);  // reference in C++17
    void pop_back();

    template <class... Args>
    iterator emplace(const_iterator position, Args&&... args);
    iterator insert(const_iterator position, const value_type& x);
    iterator insert(const_iterator position, value_type&& x);
    iterator insert(const_iterator position, size_type n, const value_type& x);
    template <class InputIterator>
    iterator insert(const_iterator position, InputIterator first, InputIterator last);
    iterator insert(const_iterator position, std::initializer_list<value_type> il);

    iterator erase(const_iterator position);
    iterator erase(const_iterator first, const_iterator last);

    void clear() noexcept {
        jstl::destroy(__begin_, __end_);
        __end_ = __begin_;
    }

    void resize(size_type sz) {
        resize(sz, value_type());
    }
    void resize(size_type sz, const value_type& c) {
        if (sz < size()) {
            jstl::destroy(__begin_ + sz, __end_);
            __end_ = __begin_ + sz;
        } else if (sz > size()) {
            if (sz > capacity()) {
                reserve(sz);
            }
            jstl::uninitialized_fill_n(__end_, sz - size(), c, alloc());
            __end_ += (sz - size());
        }
    }

    void swap(vector&) noexcept(
        allocator_traits<allocator_type>::propagate_on_container_swap::value ||
        allocator_traits<allocator_type>::is_always_equal::value) {
        // todo swap implementation
        // using jstl::swap;
        // swap(__begin_, __begin_);
        // swap(__end_, __end_);
        // swap(__cap_and_alloc_, __cap_and_alloc_);
    }

    bool __invariants() const {
        return __begin_ <= __end_ && __end_ <= cap();
    }

private:
    pointer __begin_ = nullptr;
    pointer __end_ = nullptr;
    __compressed_pair<pointer, allocator_type> __cap_and_alloc_;

    allocator_type& alloc() noexcept {
        return __cap_and_alloc_.second();
    }
    const allocator_type& alloc() const noexcept {
        return __cap_and_alloc_.second();
    }
    pointer& cap() noexcept {
        return __cap_and_alloc_.first();
    }
    const pointer& cap() const noexcept {
        return __cap_and_alloc_.first();
    }
    void init_n(size_type n) {
        if (n == 0) {
            return;
        }

        pointer p = allocator_traits<allocator_type>::allocate(alloc(), n);
        __begin_ = p;
        __end_ = p;
        cap() = p + n;

        if constexpr (is_trivially_default_constructible_v<value_type>) {
            __end_ = __begin_ + n;
        } else {
            try {
                jstl::uninitialized_default_construct_n(__begin_, n);
                __end_ = __begin_ + n;
            } catch (...) {
                allocator_traits<allocator_type>::deallocate(alloc(), __begin_, n);
                __begin_ = __end_ = nullptr;
                cap() = nullptr;
                throw;
            }
        }
    }
    void init_n(size_type n, const value_type& value) {
        if (n == 0) {
            return;
        }

        pointer p = allocator_traits<allocator_type>::allocate(alloc(), n);
        __begin_ = p;
        __end_ = p;
        cap() = p + n;

        try {
            jstl::uninitialized_fill_n(__begin_, n, value);
            __end_ = __begin_ + n;
        } catch (...) {
            allocator_traits<allocator_type>::deallocate(alloc(), __begin_, n);
            __begin_ = __end_ = nullptr;
            cap() = nullptr;
            throw;
        }
    }
};

}  // namespace jstl

#endif