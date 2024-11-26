#ifndef _MY_TYPE_TRAITS_REMOVE_REFERENCE_H
#define _MY_TYPE_TRAITS_REMOVE_REFERENCE_H

namespace my {

template <class _Tp>
struct remove_reference {
    using type = __remove_reference_t(_Tp);
};

template <class _Tp>
using remove_reference_t = __remove_reference_t(_Tp);

}  // namespace my

#endif