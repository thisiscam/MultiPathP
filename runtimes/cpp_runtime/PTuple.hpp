#ifndef PTUPLE_HPP
#define PTUPLE_HPP

#include "PTypePtr.h"

namespace RUNTIME_NAMESPACE {

struct PTupleIndexer {
    struct V0 { };
    struct V1 { };
    struct V2 { };
    struct V3 { };
};

template<typename ...Ts>
class PTuple;

template <typename T0>
class PTuple<T0> final : public AbstractPTypePtr<PTuple<T0>> {

public:
    PTuple() = default;
    
    PTuple(const T0& _v0) noexcept:_v0(_v0) { }

    PTuple(T0&& _v0) noexcept:_v0(std::move(_v0)) { }

    template<typename T0p>
    operator PTuple<T0p>() const {
        return PTuple<T0p>(_v0);
    }

    inline Bool operator == (const PTuple<T0>& other) const {
        return _v0 == other._v0;
    }

    inline Bool operator != (const PTuple<T0>& other) const {
        return !(*this == other);
    }

    template<typename V, typename Head, typename ...Rest>
    inline void set(const V& val, PTupleIndexer::V0 _unused, Head head, Rest... rest) { _v0.set(val, head, rest...); }

    inline const T0& v0() const { return _v0; }

    inline void set(const T0& val, PTupleIndexer::V0 _unused) { _v0 = val; }

    T0 _v0;

#ifdef USE_VALUE_SUMMARY
    class Builder {
        typename T0::Builder _v0;
    public:
        inline Builder& addValue(const Bdd& pred, const PTuple<T0>& rhs) {
            _v0.addValue(pred, rhs._v0);
            return *this;
        }
        inline PTuple<T0> build() {
            return PTuple<T0>(_v0.build());
        }
    };
#endif
};

template <typename T0, typename T1>
class PTuple<T0, T1> final : public AbstractPTypePtr<PTuple<T0, T1>> {

public:
    PTuple() = default;
    
    PTuple(const T0& _v0, const T1& _v1) noexcept:_v0(_v0),_v1(_v1) { }

    PTuple(T0&& _v0, T1&& _v1) noexcept:_v0(std::move(_v0)),_v1(std::move(_v1)) { }

    template<typename T0p, typename T1p>
    operator PTuple<T0p, T1p>() const {
        return PTuple<T0p, T1p>(_v0, _v1);
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

#ifdef USE_VALUE_SUMMARY
    class Builder {
        typename T0::Builder _v0;
        typename T1::Builder _v1;
    public:
        inline Builder& addValue(const Bdd& pred, const PTuple<T0, T1>& rhs) {
            _v0.addValue(pred, rhs._v0);
            _v1.addValue(pred, rhs._v1);
            return *this;
        }
        inline PTuple<T0, T1> build() {
            return PTuple<T0, T1>(_v0.build(), _v1.build());
        }
    };
#endif
};

template <typename T0, typename T1, typename T2>
class PTuple<T0, T1, T2> final : public AbstractPTypePtr<PTuple<T0, T1, T2>> {

public:
    PTuple() = default;

    PTuple(const T0& _v0, const T1& _v1, const T2& _v2) noexcept:_v0(_v0),_v1(_v1),_v2(_v2) { }

    PTuple(T0&& _v0, T1&& _v1, T2&& _v2) noexcept:_v0(std::move(_v0)),_v1(std::move(_v1)),_v2(std::move(_v2)) { }

    template<typename T0p, typename T1p, typename T2p>
    operator PTuple<T0p, T1p, T2p>() const {
        return PTuple<T0p, T1p, T2p>(_v0, _v1, _v2);
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

#ifdef USE_VALUE_SUMMARY
    class Builder {
        typename T0::Builder _v0;
        typename T1::Builder _v1;
        typename T2::Builder _v2;
    public:
        inline Builder& addValue(const Bdd& pred, const PTuple<T0, T1, T2>& rhs) {
            _v0.addValue(pred, rhs._v0);
            _v1.addValue(pred, rhs._v1);
            _v2.addValue(pred, rhs._v2);
            return *this;
        }
        inline PTuple<T0, T1, T2> build() {
            return PTuple<T0, T1, T2>(_v0.build(), _v1.build(), _v2.build());
        }
    };
#endif

};

template <typename T0, typename T1, typename T2, typename T3>
class PTuple<T0, T1, T2, T3> final : public AbstractPTypePtr<PTuple<T0, T1, T2, T3>> {

public:
    PTuple() = default;

    PTuple(const T0& _v0, const T1& _v1, const T2& _v2, const T3& _v3) noexcept:_v0(_v0),_v1(_v1),_v2(_v2),_v3(_v3) { }

    PTuple(T0&& _v0, T1&& _v1, T2&& _v2, T3&& _v3) noexcept:_v0(std::move(_v0)),_v1(std::move(_v1)),_v2(std::move(_v2)),_v3(std::move(_v3)) { }

    template<typename T0p, typename T1p, typename T2p, typename T3p>
    operator PTuple<T0p, T1p, T2p, T3p>() const {
        return PTuple<T0p, T1p, T2p, T3p>(_v0, _v1, _v2, _v3);
    }

    inline Bool operator == (const PTuple<T0, T1, T2, T3>& other) const {
        return _v0 == other._v0 & _v1 == other._v1 & _v2 == other._v2 & _v3 == other._v3;
    }

    inline Bool operator != (const PTuple<T0, T1, T2, T3>& other) const {
        return !(*this == other);
    }

    inline const T0& v0() const { return _v0; }
    inline const T1& v1() const { return _v1; }
    inline const T2& v2() const { return _v2; }
    inline const T3& v3() const { return _v3; }

    template<typename V, typename Head, typename ...Rest>
    inline void set(const V& val, PTupleIndexer::V0 _unused, Head head, Rest... rest) { _v0.set(val, head, rest...); }
    template<typename V, typename Head, typename ...Rest>
    inline void set(const V& val, PTupleIndexer::V1 _unused, Head head, Rest... rest) { _v1.set(val, head, rest...); }
    template<typename V, typename Head, typename ...Rest>
    inline void set(const V& val, PTupleIndexer::V2 _unused, Head head, Rest... rest) { _v2.set(val, head, rest...); }
    template<typename V, typename Head, typename ...Rest>
    inline void set(const V& val, PTupleIndexer::V3 _unused, Head head, Rest... rest) { _v3.set(val, head, rest...); }

    inline void set(const T0& val, PTupleIndexer::V0 _unused) { _v0 = val; }
    inline void set(const T1& val, PTupleIndexer::V1 _unused) { _v1 = val; }
    inline void set(const T2& val, PTupleIndexer::V2 _unused) { _v2 = val; }
    inline void set(const T3& val, PTupleIndexer::V3 _unused) { _v3 = val; }

    T0 _v0;
    T1 _v1;
    T2 _v2;
    T3 _v3;

#ifdef USE_VALUE_SUMMARY
    class Builder {
        typename T0::Builder _v0;
        typename T1::Builder _v1;
        typename T2::Builder _v2;
        typename T3::Builder _v3;
    public:
        inline Builder& addValue(const Bdd& pred, const PTuple<T0, T1, T2, T3>& rhs) {
            _v0.addValue(pred, rhs._v0);
            _v1.addValue(pred, rhs._v1);
            _v2.addValue(pred, rhs._v2);
            _v3.addValue(pred, rhs._v3);
            return *this;
        }
        PTuple<T0, T1, T2, T3> build() {
            return PTuple<T0, T1, T2, T3>(_v0.build(), _v1.build(), _v2.build(), _v3.build());
        }
    };
#endif

};
};

#endif