#ifndef PTUPLE_HPP
#define PTUPLE_HPP

#include "PTypePtr.h"

namespace basic_cpp_runtime {

template<typename ...Ts>
class PTuple;

template <typename T0, typename T1>
class PTuple<T0, T1> final : public PTypePtr {

public:
    PTuple() = default;
    
    PTuple(const T0& _v0, const T1& _v1):_v0(_v0),_v1(_v1) { }

    template<typename T0p, typename T1p>
    operator PTuple<T0p, T1p>() {
        return PTuple<T0p, T1p>(static_cast<T0p>(_v0), static_cast<T1p>(_v1));
    }

    inline bool operator == (const PTuple<T0, T1>& other) const {
        return _v0 == other._v0 && _v1 == other._v1;
    }

    inline bool operator != (const PTuple<T0, T1>& other) const {
        return !(*this == other);
    }


    const T0& v0() { return _v0; }
    const T1& v1() { return _v1; }

    T0& v0l() { return _v0; }
    T1& v1l() { return _v1; }

    T0 _v0;
    T1 _v1;
};

template <typename T0, typename T1, typename T2>
class PTuple<T0, T1, T2> final : public PTypePtr {

public:
    PTuple() = default;

    PTuple(const T0& _v0, const T1& _v1, const T2& _v2):_v0(_v0),_v1(_v1),_v2(_v2) { }

    template<typename T0p, typename T1p, typename T2p>
    operator PTuple<T0p, T1p, T2p>() {
        return PTuple<T0p, T1p>(static_cast<T0p>(_v0), static_cast<T1p>(_v1), static_cast<T2p>(_v2));
    }

    inline bool operator == (const PTuple<T0, T1, T2>& other) const {
        return _v0 == other._v0 && _v1 == other._v1 && _v2 == other._v2;
    }

    inline bool operator != (const PTuple<T0, T1, T2>& other) const {
        return !(*this == other);
    }

    const T0& v0() { return _v0; }
    const T1& v1() { return _v1; }
    const T2& v2() { return _v2; }

    T0& v0l() { return _v0; }
    T1& v1l() { return _v1; }
    T2& v2l() { return _v2; }

    T0 _v0;
    T1 _v1;
    T2 _v2;
};


};

#endif