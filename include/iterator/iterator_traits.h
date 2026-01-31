#ifndef _JSTL_ITERATOR_ITERATOR_TRAITS_H
#define _JSTL_ITERATOR_ITERATOR_TRAITS_H

#include <cstddef>

#include "type_traits/detected_or.h"
#include "type_traits/integral_constant.h"
#include "type_traits/is_convertible.h"
#include "type_traits/nat.h"
#include "type_traits/remove_cv.h"

namespace jstl {

// Iterator tags
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

// 1. a iterator should have these typedefs
template <class Tp>
struct __has_iterator_typedefs {
private:
    template <class _Up>
    static false_type __test(...);
    template <class _Up>
    static true_type __test(typename _Up::iterator_category* = nullptr,
                            typename _Up::difference_type* = nullptr,
                            typename _Up::value_type* = nullptr, typename _Up::reference* = nullptr,
                            typename _Up::pointer* = nullptr);

public:
    static const bool value =
        decltype(__test<Tp>(nullptr, nullptr, nullptr, nullptr, nullptr))::value;
};

template <class Iter, bool>
struct __iterator_traits_impl {};

// If passed 1 and 2, defines the typedefs
template <class Iter>
struct __iterator_traits_impl<Iter, true> {
    typedef typename Iter::difference_type difference_type;
    typedef typename Iter::value_type value_type;
    typedef typename Iter::pointer pointer;
    typedef typename Iter::reference reference;
    typedef typename Iter::iterator_category iterator_category;
};

template <class Iter, bool>
struct __iterator_traits {};

// 2. this iterator's iterator_category should be derived from
// input_iterator_tag or output_iterator_tag
template <class Iter>
struct __iterator_traits<Iter, true>
    : __iterator_traits_impl<
          Iter, is_convertible<typename Iter::iterator_category, input_iterator_tag>::value ||
                    is_convertible<typename Iter::iterator_category, output_iterator_tag>::value> {
};

template <class Iter>
struct iterator_traits : __iterator_traits<Iter, __has_iterator_typedefs<Iter>::value> {};

// Specialization for pointer types
template <class Tp>
struct iterator_traits<Tp*> {
    typedef ptrdiff_t difference_type;
    typedef remove_cv_t<Tp> value_type;
    typedef Tp* pointer;
    typedef Tp& reference;
    typedef random_access_iterator_tag iterator_category;
};

template <class Tp>
using __iterator_category = typename Tp::iterator_category;

template <class Tp, class _Up>
using __has_iterator_category_convertible_to =
    is_convertible<detected_or_t<_nat, __iterator_category, iterator_traits<Tp> >, _Up>;

template <class Tp>
using __has_input_iterator_category =
    __has_iterator_category_convertible_to<Tp, input_iterator_tag>;

template <class Tp>
using __has_forward_iterator_category =
    __has_iterator_category_convertible_to<Tp, forward_iterator_tag>;

template <class Tp>
using __has_bidirectional_iterator_category =
    __has_iterator_category_convertible_to<Tp, bidirectional_iterator_tag>;

template <class Tp>
using __has_random_access_iterator_category =
    __has_iterator_category_convertible_to<Tp, random_access_iterator_tag>;

}  // namespace jstl

#endif