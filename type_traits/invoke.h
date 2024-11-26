#ifndef _MY_TYPE_TRAITS_INVOKE_H
#define _MY_TYPE_TRAITS_INVOKE_H

#include <__utility/declval.h>  //todo...

#include "type_traits/enable_if.h"
#include "type_traits/integral_constant.h"
#include "type_traits/is_void.h"
#include "type_traits/nat.h"


namespace my {

template <class Fp, class... Args>
inline constexpr decltype(std::declval<Fp>()(std::declval<Args>()...))
_invoke(Fp&& fp, Args&&... args) noexcept(
    noexcept(static_cast<Fp&&>(fp)(static_cast<Args&&>(args)...))) {
    return static_cast<Fp&&>(fp)(static_cast<Args&&>(args)...);
}

template <class Ret, class Fp, class... Args>
struct _invokable_r {
    template <class XFp, class... XArgs>
    static decltype(_invoke(std::declval<XFp>(), std::declval<XArgs>()...))
    _try_call(int);
    template <class XFp, class... XArgs>
    static _nat _try_call(...);

    using _result = decltype(_try_call<Fp, Args...>(0));
    using type = false_type;  // todo...
    static const bool value = type::value;
};

template <class Fp, class... Args>
using _invokable = _invokable_r<void, Fp, Args...>;

template <bool _IsInvokable, bool _IsCVVoid, class Ret, class Fp, class... Args>
struct __nothrow_invokable_r_imp {
    static const bool value = false;
};

template <class Ret, class Fp, class... Args>
struct __nothrow_invokable_r_imp<true, false, Ret, Fp, Args...> {
    typedef __nothrow_invokable_r_imp _ThisT;

    template <class _Tp>
    static void _test_noexcept(_Tp) noexcept;

    static const bool value = noexcept(_test_noexcept<Ret>(
        _invoke(std::declval<Fp>(), std::declval<Args>()...)));
};

template <class Ret, class Fp, class... Args>
struct __nothrow_invokable_r_imp<true, true, Ret, Fp, Args...> {
    static const bool value =
        noexcept(_invoke(std::declval<Fp>(), std::declval<Args>()...));
};

template <class Fp, class... Args>
struct _invoke_of
    : public enable_if<_invokable<Fp, Args...>::value,
                       typename _invokable_r<void, Fp, Args...>::_result> {};

template <class Ret, class Fp, class... Args>
using __nothrow_invokable_r =
    __nothrow_invokable_r_imp<_invokable_r<Ret, Fp, Args...>::value,
                              is_void<Ret>::value, Ret, Fp, Args...>;

template <class Fp, class... Args>
using __nothrow_invokable =
    __nothrow_invokable_r_imp<_invokable<Fp, Args...>::value, true, void, Fp,
                              Args...>;

template <class Fn, class... Args>
struct is_nothrow_invocable
    : integral_constant<bool, __nothrow_invokable<Fn, Args...>::value> {};

template <class Ret, class Fn, class... Args>
struct _LIBCPP_TEMPLATE_VIS is_nothrow_invocable_r
    : integral_constant<bool, __nothrow_invokable_r<Ret, Fn, Args...>::value> {
};

template <class Fn, class... Args>
inline constexpr bool is_nothrow_invocable_v =
    is_nothrow_invocable<Fn, Args...>::value;

template <class Ret, class Fn, class... Args>
inline constexpr bool is_nothrow_invocable_r_v =
    is_nothrow_invocable_r<Ret, Fn, Args...>::value;

template <class Fn, class... Args>
struct invoke_result : _invoke_of<Fn, Args...> {};

template <class Fn, class... Args>
using invoke_result_t = typename invoke_result<Fn, Args...>::type;

}  // namespace my

#endif