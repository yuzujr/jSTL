#ifndef _JSTL_TYPE_TRAITS_DETECTED_OR_H
#define _JSTL_TYPE_TRAITS_DETECTED_OR_H

#include "type_traits/void_t.h"

namespace jstl {

template <class Default, class AlwaysVoid, template <class...> class Op, class... Args>
struct detected_or {
    using type = Default;
};

// only when `Op<Args...>` is well-formed
template <class Default, template <class...> class Op, class... Args>
struct detected_or<Default, void_t<Op<Args...>>, Op, Args...> {
    using type = Op<Args...>;
};

template <class Default, template <class...> class Op, class... Args>
using detected_or_t = typename detected_or<Default, void, Op, Args...>::type;

}  // namespace jstl

#endif