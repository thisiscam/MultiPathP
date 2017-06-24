#ifndef PTUPLE_HPP
#define PTUPLE_HPP

#include "PTypePtr.h"

namespace RUNTIME_NAMESPACE {

struct PTupleIndexer {
    struct V0 { };
    struct V1 { };
    struct V2 { };
};

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

    template<typename V, typename Head, typename ...Rest>
    inline void set(const V& val, PTupleIndexer::V0 _unused, Head head, Rest... rest) { _v0.set(val, head, rest...); }
    template<typename V, typename Head, typename ...Rest>
    inline void set(const V& val, PTupleIndexer::V1 _unused, Head head, Rest... rest) { _v1.set(val, head, rest...); }

    inline const T0& v0() const { return _v0; }
    inline const T1& v1() const { return _v1; }

    inline void set(const T0& val, PTupleIndexer::V0 _unused) { _v0 = val; }
    inline void set(const T1& val, PTupleIndexer::V1 _unused) { _v1 = val; }

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

    template<typename V, typename Head, typename ...Rest>
    inline void set(const V& val, PTupleIndexer::V0 _unused, Head head, Rest... rest) { _v0.set(val, head, rest...); }
    template<typename V, typename Head, typename ...Rest>
    inline void set(const V& val, PTupleIndexer::V1 _unused, Head head, Rest... rest) { _v1.set(val, head, rest...); }
    template<typename V, typename Head, typename ...Rest>
    inline void set(const V& val, PTupleIndexer::V2 _unused, Head head, Rest... rest) { _v2.set(val, head, rest...); }

    inline void set(const T0& val, PTupleIndexer::V0 _unused) { _v0 = val; }
    inline void set(const T1& val, PTupleIndexer::V1 _unused) { _v1 = val; }
    inline void set(const T2& val, PTupleIndexer::V2 _unused) { _v2 = val; }

    T0 _v0;
    T1 _v1;
    T2 _v2;
};


};

#endif