#ifndef _JSTL_TUPLE_H
#define _JSTL_TUPLE_H

#include <cstddef>
#include <tuple>    //todo:tuple
#include <utility>  //todo:index_sequence

#include "functional/invoke.h"
#include "type_traits/remove_reference.h"
#include "utility/forward.h"

namespace jstl {

// apply
template <class Callable, class Tuple, size_t... Index>
constexpr decltype(auto) Apply_impl(Callable&& obj, Tuple&& tuple, std::index_sequence<Index...>) {
    return invoke(forward<Callable>(obj), std::get<Index>(forward<Tuple>(tuple))...);
}

template <class Callable, class Tuple>
constexpr decltype(auto) apply(Callable&& obj, Tuple&& tuple) {
    return Apply_impl(forward<Callable>(obj), forward<Tuple>(tuple),
                      std::make_index_sequence<std::tuple_size_v<remove_reference_t<Tuple>>>{});
}

}  // namespace jstl

#endif