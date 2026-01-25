#ifndef _JSTL_ITERATOR_ITERATOR_H
#define _JSTL_ITERATOR_ITERATOR_H

#include <cstddef>

namespace jstl {

template <class Category, class Tp, class Distance = ptrdiff_t,
          class Pointer = Tp*, class Reference = Tp&>
struct iterator {
    typedef Tp value_type;
    typedef Distance difference_type;
    typedef Pointer pointer;
    typedef Reference reference;
    typedef Category iterator_category;
};

}  // namespace jstl

#endif