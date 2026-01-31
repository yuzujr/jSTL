#ifndef _JSTL_MEMORY_COMPRESSED_PAIR_H
#define _JSTL_MEMORY_COMPRESSED_PAIR_H

#include "type_traits/is_empty.h"
#include "type_traits/is_final.h"
#include "utility/forward.h"

namespace jstl {

template <typename Tp, int Idx, bool CanBeEmptyBase = is_empty_v<Tp> && !is_final_v<Tp>>
struct __compressed_pair_elem {
    Tp __value_;
    __compressed_pair_elem() = default;
    template <class... _Args>
    explicit __compressed_pair_elem(_Args&&... __args) : __value_(forward<_Args>(__args)...) {}

    Tp& __get() {
        return __value_;
    }
    const Tp& __get() const {
        return __value_;
    }
};

template <typename Tp, int Idx>
struct __compressed_pair_elem<Tp, Idx, true> : private Tp {
    __compressed_pair_elem() = default;
    template <class... _Args>
    explicit __compressed_pair_elem(_Args&&... __args) : Tp(forward<_Args>(__args)...) {}

    Tp& __get() {
        return *this;
    }
    const Tp& __get() const {
        return *this;
    }
};

template <typename _T1, typename _T2>
class __compressed_pair : private __compressed_pair_elem<_T1, 0>,
                          private __compressed_pair_elem<_T2, 1> {
private:
    using _Base1 = __compressed_pair_elem<_T1, 0>;
    using _Base2 = __compressed_pair_elem<_T2, 1>;

public:
    __compressed_pair() = default;

    template <typename _U1, typename _U2>
    __compressed_pair(_U1&& __u1, _U2&& __u2)
        : _Base1(forward<_U1>(__u1)), _Base2(forward<_U2>(__u2)) {}

    _T1& first() {
        return static_cast<_Base1&>(*this).__get();
    }

    _T2& second() {
        return static_cast<_Base2&>(*this).__get();
    }

    const _T1& first() const {
        return static_cast<const _Base1&>(*this).__get();
    }

    const _T2& second() const {
        return static_cast<const _Base2&>(*this).__get();
    }
};

}  // namespace jstl

#endif