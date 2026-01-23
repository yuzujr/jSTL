#ifndef _JSTL_TYPE_TRAITS_NAT_H
#define _JSTL_TYPE_TRAITS_NAT_H

namespace jstl {

struct _nat {
    _nat() = delete;
    _nat(const _nat&) = delete;
    _nat& operator=(const _nat&) = delete;
    ~_nat() = delete;
};

}  // namespace jstl

#endif