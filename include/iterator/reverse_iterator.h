#ifndef _JSTL_ITERATOR_REVERSE_ITERATOR_H
#define _JSTL_ITERATOR_REVERSE_ITERATOR_H

#include "iterator/iterator_traits.h"
#include "memory/addressof.h"
#include "type_traits/conditional.h"
#include "type_traits/enable_if.h"
#include "type_traits/is_assignable.h"
#include "type_traits/is_convertible.h"
#include "type_traits/is_same.h"

namespace jstl {
template <class Iter>
class reverse_iterator {
protected:
    Iter current;

public:
    using iterator_type = Iter;
    using iterator_category = conditional_t<__has_random_access_iterator_category<Iter>::value,
                                            random_access_iterator_tag,
                                            typename iterator_traits<Iter>::iterator_category>;
    using pointer = typename iterator_traits<Iter>::pointer;
    using value_type = typename iterator_traits<Iter>::value_type;
    using difference_type = typename iterator_traits<Iter>::difference_type;
    using reference = typename iterator_traits<Iter>::reference;

    constexpr reverse_iterator() : current() {}

    constexpr explicit reverse_iterator(Iter x) : current(x) {}

    // converting ctor
    template <
        class _Up,
        enable_if_t<!is_same<_Up, Iter>::value && is_convertible<_Up const&, Iter>::value, int> = 0>
    constexpr reverse_iterator(const reverse_iterator<_Up>& __u) : current(__u.base()) {}

    template <class _Up,
              enable_if_t<!is_same<_Up, Iter>::value && is_convertible<_Up const&, Iter>::value &&
                              is_assignable<Iter&, _Up const&>::value,
                          int> = 0>
    constexpr reverse_iterator& operator=(const reverse_iterator<_Up>& __u) {
        current = __u.base();
        return *this;
    }

    constexpr Iter base() const {
        return current;
    }

    constexpr reference operator*() {
        Iter tmp = current;
        return *--tmp;
    }

    constexpr pointer operator->() const {
        return addressof(operator*());
    }

    constexpr reverse_iterator& operator++() {
        --current;
        return *this;
    }
    constexpr reverse_iterator operator++(int) {
        reverse_iterator __tmp(*this);
        --current;
        return __tmp;
    }
    constexpr reverse_iterator& operator--() {
        ++current;
        return *this;
    }
    constexpr reverse_iterator operator--(int) {
        reverse_iterator __tmp(*this);
        ++current;
        return __tmp;
    }
    constexpr reverse_iterator operator+(difference_type n) const {
        return reverse_iterator(current - n);
    }
    constexpr reverse_iterator& operator+=(difference_type n) {
        current -= n;
        return *this;
    }
    constexpr reverse_iterator operator-(difference_type n) const {
        return reverse_iterator(current + n);
    }
    constexpr reverse_iterator& operator-=(difference_type n) {
        current += n;
        return *this;
    }
    constexpr reference operator[](difference_type n) const {
        return *(*this + n);
    }
};

template <class Iter1, class Iter2>
inline constexpr bool operator==(const reverse_iterator<Iter1>& x,
                                 const reverse_iterator<Iter2>& y) {
    return x.base() == y.base();
}

template <class Iter1, class Iter2>
inline constexpr bool operator<(const reverse_iterator<Iter1>& x,
                                const reverse_iterator<Iter2>& y) {
    return x.base() > y.base();
}

template <class Iter1, class Iter2>
inline constexpr bool operator!=(const reverse_iterator<Iter1>& x,
                                 const reverse_iterator<Iter2>& y) {
    return x.base() != y.base();
}

template <class Iter1, class Iter2>
inline constexpr bool operator>(const reverse_iterator<Iter1>& x,
                                const reverse_iterator<Iter2>& y) {
    return x.base() < y.base();
}

template <class Iter1, class Iter2>
inline constexpr bool operator>=(const reverse_iterator<Iter1>& x,
                                 const reverse_iterator<Iter2>& y) {
    return x.base() <= y.base();
}

template <class Iter1, class Iter2>
inline constexpr bool operator<=(const reverse_iterator<Iter1>& x,
                                 const reverse_iterator<Iter2>& y) {
    return x.base() >= y.base();
}

template <class Iter1, class Iter2>
inline constexpr auto operator-(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
    -> decltype(y.base() - x.base()) {
    return y.base() - x.base();
}

template <class Iter>
inline constexpr reverse_iterator<Iter> operator+(
    typename reverse_iterator<Iter>::difference_type n, const reverse_iterator<Iter>& x) {
    return reverse_iterator<Iter>(x.base() - n);
}

template <class Iter>
inline constexpr reverse_iterator<Iter> make_reverse_iterator(Iter i) {
    return reverse_iterator<Iter>(i);
}

// todo: specialization for __unwrap_iter_impl<reverse_iterator<reverse_iterator<Iter>>, __b>
// wait for unwrap_iterator in algorithm

}  // namespace jstl

#endif