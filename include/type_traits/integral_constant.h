#ifndef _JSTL_TYPE_TRAITS_INTEGRAL_CONSTANT_H
#define _JSTL_TYPE_TRAITS_INTEGRAL_CONSTANT_H

namespace jstl {

template <class Tp, Tp Val>
struct integral_constant {
    static constexpr Tp value = Val;
    typedef Tp value_type;
    // injected class name, equivalent to
    // `typedef integral_constant<Tp, Val> integral_constant`;
    // see [https://en.cppreference.com/w/cpp/language/injected-class-name.html]
    typedef integral_constant type;
    constexpr operator value_type() const noexcept {
        return value;
    }
    constexpr value_type operator()() const noexcept {
        return value;
    }
};

template <class Tp, Tp Val>
constexpr Tp integral_constant<Tp, Val>::value;

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

template <bool Val>
using bool_constant = integral_constant<bool, Val>;

}  // namespace jstl

#endif