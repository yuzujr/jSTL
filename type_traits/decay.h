#ifndef _MY_TYPE_TRAITS_DECAY_H
#define _MY_TYPE_TRAITS_DECAY_H

#include "type_traits/add_pointer.h"
#include "type_traits/conditional.h"
#include "type_traits/is_array.h"
#include "type_traits/is_function.h"
#include "type_traits/is_referenceable.h"
#include "type_traits/remove_cv.h"
#include "type_traits/remove_extent.h"
#include "type_traits/remove_reference.h"

namespace my {

namespace detail {

template <class Up, bool>
struct _decay {
    typedef remove_cv_t<Up> type;
};
template <class Up>
struct _decay<Up, true> {
    typedef conditional_t<
        is_array<Up>::value, add_pointer_t<remove_extent_t<Up>>,
        conditional_t<is_function<Up>::value, typename add_pointer<Up>::type,
                      remove_cv_t<Up>>>
        type;
};

}  // namespace detail

template <class Tp>
struct decay {
private:
    typedef remove_reference_t<Tp> Up;

public:
    typedef typename detail::_decay<Up, is_referenceable<Up>::value>::type tpye;
};

template <class Tp>
using decay_t = typename decay<Tp>::type;

}  // namespace my

#endif