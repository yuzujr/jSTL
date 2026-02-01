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
#include "utility/forward.h"
#include "utility/move.h"
#include "utility/swap.h"

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
    template <class InputIterator,
              class = typename std::enable_if<!std::is_integral<InputIterator>::value>::type>
    vector(InputIterator first, InputIterator last, const allocator_type& __a = allocator_type())
        : __cap_and_alloc_{nullptr, __a} {
        size_type n = static_cast<size_type>(last - first);
        if (n > 0) {
            reserve(n);
            jstl::uninitialized_copy(first, last, __begin_);
            __end_ = __begin_ + n;
        }
    }
    vector(const vector& x)
        : __cap_and_alloc_{
              nullptr,
              allocator_traits<allocator_type>::select_on_container_copy_construction(x.alloc())} {
        reserve(x.size());
        jstl::uninitialized_copy(x.__begin_, x.__end_, __begin_);
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

    vector& operator=(const vector& x) {
        if (this != &x) {
            clear();
            if (allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value &&
                alloc() != x.alloc()) {
                alloc() = x.alloc();
            }
            reserve(x.size());
            jstl::uninitialized_copy(x.__begin_, x.__end_, __begin_);
            __end_ = __begin_ + x.size();
        }
        return *this;
    }
    vector& operator=(vector&& x) noexcept(
        allocator_type::propagate_on_container_move_assignment::value ||
        allocator_type::is_always_equal::value) {
        if (this != &x) {
            clear();
            if (allocator_traits<allocator_type>::propagate_on_container_move_assignment::value) {
                alloc() = jstl::move(x.alloc());
            }
            __begin_ = x.__begin_;
            __end_ = x.__end_;
            cap() = x.cap();
            x.__begin_ = nullptr;
            x.__end_ = nullptr;
            x.cap() = nullptr;
        }
        return *this;
    }

    vector& operator=(std::initializer_list<value_type> il) {
        assign(il);
        return *this;
    }

    template <class InputIterator,
              class = typename std::enable_if<!std::is_integral<InputIterator>::value>::type>
    void assign(InputIterator first, InputIterator last) {
        size_type n = static_cast<size_type>(last - first);
        if (n > capacity()) {
            vector tmp(first, last, alloc());
            swap(tmp);
        } else if (n > size()) {
            // Copy to existing elements
            InputIterator mid = first;
            for (size_type i = 0; i < size(); ++i) {
                __begin_[i] = *mid;
                ++mid;
            }
            // Construct new elements
            jstl::uninitialized_copy(mid, last, __end_);
            __end_ = __begin_ + n;
        } else {
            // Copy elements up to n
            for (size_type i = 0; i < n; ++i) {
                __begin_[i] = *first;
                ++first;
            }
            // Destroy excess elements
            jstl::destroy(__begin_ + n, __end_);
            __end_ = __begin_ + n;
        }
    }
    void assign(size_type n, const value_type& u) {
        if (n > capacity()) {
            vector tmp(n, u, alloc());
            swap(tmp);
        } else if (n > size()) {
            // Fill existing elements
            for (size_type i = 0; i < size(); ++i) {
                __begin_[i] = u;
            }
            // Construct new elements
            jstl::uninitialized_fill_n(__end_, n - size(), u);
            __end_ = __begin_ + n;
        } else {
            // Fill elements up to n
            for (size_type i = 0; i < n; ++i) {
                __begin_[i] = u;
            }
            // Destroy excess elements
            jstl::destroy(__begin_ + n, __end_);
            __end_ = __begin_ + n;
        }
    }
    void assign(std::initializer_list<value_type> il) {
        assign(il.begin(), il.end());
    }

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
    void reserve(size_type n) {
        if (n > capacity()) {
            pointer new_begin = allocator_traits<allocator_type>::allocate(alloc(), n);
            size_type old_size = size();
            try {
                jstl::uninitialized_move(__begin_, __end_, new_begin);
            } catch (...) {
                alloc().deallocate(new_begin, n);
                throw;
            }
            if (__begin_) {
                jstl::destroy(__begin_, __end_);
                alloc().deallocate(__begin_, capacity());
            }
            __begin_ = new_begin;
            __end_ = new_begin + old_size;
            cap() = new_begin + n;
        }
    }
    void shrink_to_fit() noexcept {
        if (size() < capacity()) {
            try {
                vector tmp(jstl::move(*this));
                swap(tmp);
            } catch (...) {
                // noexcept specification, swallow exception
            }
        }
    }

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
            reserve(size() == 0 ? 1 : size() * 2);
        }
        allocator_traits<allocator_type>::construct(alloc(), __end_, x);
        ++__end_;
    }
    void push_back(value_type&& x) {
        if (__end_ == cap()) {
            reserve(size() == 0 ? 1 : size() * 2);
        }
        allocator_traits<allocator_type>::construct(alloc(), __end_, jstl::move(x));
        ++__end_;
    }
    template <class... Args>
    reference emplace_back(Args&&... args) {
        if (__end_ == cap()) {
            reserve(size() == 0 ? 1 : size() * 2);
        }
        allocator_traits<allocator_type>::construct(alloc(), __end_, jstl::forward<Args>(args)...);
        ++__end_;
        return back();
    }
    void pop_back() {
        --__end_;
        allocator_traits<allocator_type>::destroy(alloc(), __end_);
    }

    template <class... Args>
    iterator emplace(const_iterator position, Args&&... args) {
        difference_type offset = position - __begin_;
        if (__end_ == cap()) {
            reserve(size() == 0 ? 1 : size() * 2);
        }
        iterator pos = __begin_ + offset;
        if (pos == __end_) {
            allocator_traits<allocator_type>::construct(alloc(), __end_,
                                                        jstl::forward<Args>(args)...);
            ++__end_;
        } else {
            value_type tmp(jstl::forward<Args>(args)...);
            allocator_traits<allocator_type>::construct(alloc(), __end_, jstl::move(*(__end_ - 1)));
            ++__end_;
            for (iterator it = __end_ - 2; it != pos; --it) {
                *it = jstl::move(*(it - 1));
            }
            *pos = jstl::move(tmp);
        }
        return __begin_ + offset;
    }
    iterator insert(const_iterator position, const value_type& x) {
        return emplace(position, x);
    }
    iterator insert(const_iterator position, value_type&& x) {
        return emplace(position, jstl::move(x));
    }
    iterator insert(const_iterator position, size_type n, const value_type& x) {
        if (n == 0) {
            return const_cast<iterator>(position);
        }
        difference_type offset = position - __begin_;
        if (size() + n > capacity()) {
            size_type new_cap = size() + n;
            if (new_cap < size() * 2) {
                new_cap = size() * 2;
            }
            reserve(new_cap);
        }
        iterator pos = __begin_ + offset;
        if (pos == __end_) {
            jstl::uninitialized_fill_n(__end_, n, x);
            __end_ += n;
        } else {
            size_type elems_after = __end_ - pos;
            pointer old_end = __end_;
            if (elems_after > n) {
                jstl::uninitialized_move(__end_ - n, __end_, __end_);
                __end_ += n;
                for (pointer it = old_end - 1; it >= pos + n; --it) {
                    *it = jstl::move(*(it - n));
                }
                for (size_type i = 0; i < n; ++i) {
                    *(pos + i) = x;
                }
            } else {
                jstl::uninitialized_fill_n(__end_, n - elems_after, x);
                __end_ += (n - elems_after);
                jstl::uninitialized_move(pos, old_end, __end_);
                __end_ += elems_after;
                for (pointer it = pos; it != old_end; ++it) {
                    *it = x;
                }
            }
        }
        return __begin_ + offset;
    }
    template <class InputIterator,
              class = typename std::enable_if<!std::is_integral<InputIterator>::value>::type>
    iterator insert(const_iterator position, InputIterator first, InputIterator last) {
        size_type n = static_cast<size_type>(last - first);
        if (n == 0) {
            return const_cast<iterator>(position);
        }
        difference_type offset = position - __begin_;
        if (size() + n > capacity()) {
            size_type new_cap = size() + n;
            if (new_cap < size() * 2) {
                new_cap = size() * 2;
            }
            reserve(new_cap);
        }
        iterator pos = __begin_ + offset;
        if (pos == __end_) {
            jstl::uninitialized_copy(first, last, __end_);
            __end_ += n;
        } else {
            size_type elems_after = __end_ - pos;
            pointer old_end = __end_;
            if (elems_after > n) {
                jstl::uninitialized_move(__end_ - n, __end_, __end_);
                __end_ += n;
                for (pointer it = old_end - 1; it >= pos + n; --it) {
                    *it = jstl::move(*(it - n));
                }
                for (size_type i = 0; i < n; ++i, ++first) {
                    *(pos + i) = *first;
                }
            } else {
                InputIterator mid = first;
                for (size_type i = 0; i < elems_after; ++i) {
                    ++mid;
                }
                jstl::uninitialized_copy(mid, last, __end_);
                __end_ += (n - elems_after);
                jstl::uninitialized_move(pos, old_end, __end_);
                __end_ += elems_after;
                for (pointer it = pos; it != old_end; ++it, ++first) {
                    *it = *first;
                }
            }
        }
        return __begin_ + offset;
    }
    iterator insert(const_iterator position, std::initializer_list<value_type> il) {
        return insert(position, il.begin(), il.end());
    }

    iterator erase(const_iterator position) {
        iterator pos = const_cast<iterator>(position);
        for (iterator it = pos + 1; it != __end_; ++it) {
            *(it - 1) = jstl::move(*it);
        }
        --__end_;
        allocator_traits<allocator_type>::destroy(alloc(), __end_);
        return pos;
    }
    iterator erase(const_iterator first, const_iterator last) {
        if (first == last) {
            return const_cast<iterator>(last);
        }
        iterator f = const_cast<iterator>(first);
        iterator l = const_cast<iterator>(last);
        iterator new_end = f;
        for (iterator it = l; it != __end_; ++it, ++new_end) {
            *new_end = jstl::move(*it);
        }
        jstl::destroy(new_end, __end_);
        __end_ = new_end;
        return f;
    }

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
            jstl::uninitialized_fill_n(__end_, sz - size(), c);
            __end_ += (sz - size());
        }
    }

    void swap(vector& x) noexcept(
        allocator_traits<allocator_type>::propagate_on_container_swap::value ||
        allocator_traits<allocator_type>::is_always_equal::value) {
        using jstl::swap;
        swap(__begin_, x.__begin_);
        swap(__end_, x.__end_);
        swap(__cap_and_alloc_, x.__cap_and_alloc_);
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