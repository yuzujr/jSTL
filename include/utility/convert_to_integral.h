#ifndef _JSTL_UTILITY_CONVERT_TO_INTEGRAL_H
#define _JSTL_UTILITY_CONVERT_TO_INTEGRAL_H

#include "type_traits/enable_if.h"
#include "type_traits/is_enum.h"
#include "type_traits/is_floating_point.h"
#include "type_traits/underlying_type.h"

namespace jstl {

inline int __convert_to_integral(int val) {
    return val;
}

inline unsigned __convert_to_integral(unsigned val) {
    return val;
}

inline long __convert_to_integral(long val) {
    return val;
}

inline unsigned long __convert_to_integral(unsigned long val) {
    return val;
}

inline long long __convert_to_integral(long long val) {
    return val;
}

inline unsigned long long __convert_to_integral(unsigned long long val) {
    return val;
}

template <typename _Fp, enable_if_t<is_floating_point<_Fp>::value, int> = 0>
inline long long __convert_to_integral(_Fp val) {
    return val;
}

template <class _Tp, bool = is_enum<_Tp>::value>
struct __enum_underlying_type {
    using type = underlying_type_t<_Tp>;
    typedef decltype(((type)1) + 0) promoted_type;
};

template <class _Tp>
struct __enum_underlying_type<_Tp, false> {};

template <class _Tp>
inline typename __enum_underlying_type<_Tp>::promoted_type __convert_to_integral(_Tp val) {
    return val;
}

}  // namespace jstl

#endif