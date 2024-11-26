#ifndef _MY_TYPE_TRAITS_NAT_H
#define _MY_TYPE_TRAITS_NAT_H

namespace my {

struct _nat {
    _nat() = delete;
    _nat(const _nat&) = delete;
    _nat& operator=(const _nat&) = delete;
    ~_nat() = delete;
};

}  // namespace my

#endif