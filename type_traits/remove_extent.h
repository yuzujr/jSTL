#ifndef _MY_TYPE_TRAITS_REMOVE_EXTENT_H
#define _MY_TYPE_TRAITS_REMOVE_EXTENT_H

namespace my {
template <class Tp>
struct remove_extent {
    typedef Tp type;
};
template <class Tp>
struct remove_extent<Tp[]> {
    typedef Tp type;
};
template <class Tp, size_t Np>
struct remove_extent<Tp[Np]> {
    typedef Tp type;
};

template <class Tp>
using remove_extent_t = typename remove_extent<Tp>::type;
}  // namespace my

#endif