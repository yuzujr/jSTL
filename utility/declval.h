#ifndef _MY_UTILITY_DECLVAL_H
#define _MY_UTILITY_DECLVAL_H

#include "type_traits/is_same.h"

namespace my {

template <class Tp>
Tp&& __declval(int);
template <class Tp>
Tp __declval(long);

template <class Tp>
decltype(__declval<Tp>(0)) declval() noexcept {
    static_assert(!is_same<Tp, Tp>(),
                  "my::declval can only be used in an unevaluated context. "
                  "It's likely that your current usage is trying to extract a "
                  "value from the function.");
}
}  // namespace my

#endif