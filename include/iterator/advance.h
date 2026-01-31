#ifndef _JSTL_ITERATOR_ADVANCE_H
#define _JSTL_ITERATOR_ADVANCE_H

#include "iterator/iterator_traits.h"
#include "type_traits/enable_if.h"
#include "type_traits/is_integral.h"
#include "utility/convert_to_integral.h"
#include "utility/declval.h"

namespace jstl {
//
template <class InputIter>
constexpr void __advance(InputIter& it, typename iterator_traits<InputIter>::difference_type n,
                         input_iterator_tag) {
    for (; n > 0; --n) {
        ++it;
    }
}

template <class BiDirIter>
constexpr void __advance(BiDirIter& it, typename iterator_traits<BiDirIter>::difference_type n,
                         bidirectional_iterator_tag) {
    if (n >= 0) {
        for (; n > 0; --n) {
            ++it;
        }
    } else {
        for (; n < 0; ++n) {
            --it;
        }
    }
}

template <class RandIter>
constexpr void __advance(RandIter& it, typename iterator_traits<RandIter>::difference_type n,
                         random_access_iterator_tag) {
    it += n;
}

template <class InputIter, class Distance,
          class IntegralDistance = decltype(__convert_to_integral(declval<Distance>())),
          enable_if_t<is_integral<IntegralDistance>::value, int> = 0>
void advance(InputIter& __i, Distance __orig_n) {
    typedef typename iterator_traits<InputIter>::difference_type difference_t;
    typedef typename iterator_traits<InputIter>::iterator_category category_t;
    difference_t __n = static_cast<difference_t>(__convert_to_integral(__orig_n));
    static_assert(__has_bidirectional_iterator_category<category_t>::value || __n >= 0,
                  "std::advance: Can only pass a negative `n` with a "
                  "bidirectional_iterator.");
    jstl::__advance(__i, __n, category_t());
}

}  // namespace jstl

#endif