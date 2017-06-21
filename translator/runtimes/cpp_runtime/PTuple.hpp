#ifndef PTUPLE_HPP
#define PTUPLE_HPP

#include "PTypePtr.h"
#include "Ref.hpp"

namespace RUNTIME_NAMESPACE {

template<typename ...Ts>
class PTuple;

template <typename T0, typename T1>
class PTuple<T0, T1> final : public PTypePtr {

public:
    PTuple() = default;
    
    PTuple(const T0& _v0, const T1& _v1):_v0(_v0),_v1(_v1) { }

    PTuple(T0&& _v0, T1&& _v1):_v0(std::move(_v0)),_v1(std::move(_v1)) { }

    template<typename T0p, typename T1p>
    operator PTuple<T0p, T1p>() {
        return PTuple<T0p, T1p>(static_cast<T0p>(_v0), static_cast<T1p>(_v1));
    }

    inline Bool operator == (const PTuple<T0, T1>& other) const {
        return _v0 == other._v0 & _v1 == other._v1;
    }

    inline Bool operator != (const PTuple<T0, T1>& other) const {
        return !(*this == other);
    }

    inline const T0& v0() const { return _v0; }
    inline const T1& v1() const { return _v1; }

    inline Ref<T0> v0l() { return _v0; }
    inline Ref<T1> v1l() { return _v1; }

    T0 _v0;
    T1 _v1;
};

template <typename T0, typename T1, typename T2>
class PTuple<T0, T1, T2> final : public PTypePtr {

public:
    PTuple() = default;

    PTuple(const T0& _v0, const T1& _v1, const T2& _v2):_v0(_v0),_v1(_v1),_v2(_v2) { }

    PTuple(T0&& _v0, T1&& _v1, T2&& _v2):_v0(std::move(_v0)),_v1(std::move(_v1)),_v2(std::move(_v2)) { }

    template<typename T0p, typename T1p, typename T2p>
    operator PTuple<T0p, T1p, T2p>() {
        return PTuple<T0p, T1p>(static_cast<T0p>(_v0), static_cast<T1p>(_v1), static_cast<T2p>(_v2));
    }

    inline Bool operator == (const PTuple<T0, T1, T2>& other) const {
        return _v0 == other._v0 & _v1 == other._v1 & _v2 == other._v2;
    }

    inline Bool operator != (const PTuple<T0, T1, T2>& other) const {
        return !(*this == other);
    }

    inline const T0& v0() const { return _v0; }
    inline const T1& v1() const { return _v1; }
    inline const T2& v2() const { return _v2; }

    inline Ref<T0> v0l() { return _v0; }
    inline Ref<T1> v1l() { return _v1; }
    inline Ref<T2> v2l() { return _v2; }

    T0 _v0;
    T1 _v1;
    T2 _v2;
};


template<typename T0, typename T1>
class Ref<PTuple<T0, T1>> final {
    REF_BODY(PTuple<T0, T1>)
    Ref<T0> v0l() { return value->v0l(); }
    Ref<T1> v1l() { return value->v1l(); }
};

template<typename T0, typename T1, typename T2>
class Ref<PTuple<T0, T1, T2>> final {
    REF_BODY(PTuple<T0, T1, T2>)
    Ref<T0> v0l() { return value->v0l(); }
    Ref<T1> v1l() { return value->v1l(); }
    Ref<T2> v2l() { return value->v2l(); }
};

};

#endif