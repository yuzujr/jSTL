#ifndef _JSTL_TYPE_TRAITS_CONDITIONAL_H
#define _JSTL_TYPE_TRAITS_CONDITIONAL_H

namespace jstl {

namespace detail {

template <bool>
struct _IfImpl;
template <>
struct _IfImpl<true> {
    template <class IfRes, class ElseRes>
    using Select = IfRes;
};

template <>
struct _IfImpl<false> {
    template <class IfRes, class ElseRes>
    using Select = ElseRes;
};

}  // namespace detail

template <bool Cond, class IfRes, class ElseRes>
using _If = typename detail::_IfImpl<Cond>::template Select<IfRes, ElseRes>;

template <bool Bp, class If, class Then>
struct conditional {
    using type = If;
};

template <class If, class Then>
struct conditional<false, If, Then> {
    using type = Then;
};

template <bool Bp, class IfRes, class ElseRes>
using conditional_t = typename conditional<Bp, IfRes, ElseRes>::type;
}  // namespace jstl

#endif