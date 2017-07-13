#ifndef PANY_HPP
#define PANY_HPP

#include <memory>
#include <stdexcept>
#include <typeinfo>
#include <map>
#include <iostream>
#include <utility>

#include "PTypePtr.h"
#include "SharedPointerUtils.h"
#include "utils/crossproduct.hpp"

using namespace std;

namespace RUNTIME_NAMESPACE {

template<typename T>
struct IsSimpleType
{
    static constexpr bool value = std::is_same<T, int>::value || std::is_same<T, bool>::value || std::is_same<T, PMachine*>::value;
};

template<typename, typename> struct IsCastable {
    static constexpr bool value = false;
};

template<typename T>
struct IsCastable<T, typename std::enable_if<IsSimpleType<T>::value, T>::type> {
    static constexpr bool value = true;
};

template<typename T>
struct IsCastable<PAny, T> {
    static constexpr bool value = true;
};

template<template <typename ...Ts> class Container>
struct IsCastable<Container<>, Container<>> {
    static constexpr bool value = true;
};

template<template <typename ...Ts> class Container, typename A, typename B, typename ...As, typename ...Bs>
struct IsCastable<Container<A, As...>, Container<B, Bs...>> {
    static constexpr bool value = IsCastable<A, B>::value && IsCastable<List<As...>, List<Bs...>>::value;
};

class PAny final {

#ifdef USE_VALUE_SUMMARY
using AnyDataPointer = ValueSummary<shared_ptr<PTypePtr const>>;
#else
using AnyDataPointer = shared_ptr<PTypePtr const>;
#endif

private:
    template<typename FROM, typename TO>
    static TO CastHelper(typename std::enable_if<IsCastable<FROM, TO>::value, const PAny&>::type from) {
        return getCast<FROM, TO>(from.ptr);
    }

    template<typename FROM, typename TO>
    static TO CastHelper(typename std::enable_if<!IsCastable<FROM, TO>::value, const PAny&>::type from) {
        throw bad_cast();
    }

    template<typename, typename, typename=void>
    struct EqHelperFunctor;

public:
    
    PAny() noexcept:type(NULL) { };

    PAny(const PAny& other) = default;

    PAny(PAny&& other) = default;

    PAny& operator=(const PAny& other) = default;

    PAny& operator=(PAny&& other) = default;

    template<template<typename ...> class Container, typename ...Ts>
    PAny(const Container<Ts...>& v) noexcept:
        type(&typeid(Container<Ts...>)), 
        ptr(shared_ptr<PTypePtr const>(new Container<Ts...>(v)))
    { }

    template<template<typename ...> class Container, typename ...Ts>
    PAny(Container<Ts...>&& v) noexcept:
        type(&typeid(Container<Ts...>)), 
        ptr(shared_ptr<PTypePtr const>(new Container<Ts...>(std::forward<Container<Ts...>>(v))))
    { }

    PAny(const Int& v) noexcept:
        type(&typeid(Int)), 
        i(v)
    { }

    PAny(const Bool& v) noexcept:
        type(&typeid(Bool)),
        b(v) 
    { }

    PAny(Ptr<PMachine> const & v) noexcept:
        type(&typeid(Ptr<PMachine>)),
        m(v)
    { }

    PAny(Int&& v) noexcept:
        type(&typeid(Int)), 
        i(std::move(v))
    { }

    PAny(Bool&& v) noexcept:
        type(&typeid(Bool)),
        b(std::move(v)) 
    { }

    PAny(Ptr<PMachine>&& v) noexcept:
        type(&typeid(Ptr<PMachine>)),
        m(std::move(v))
    { }

#ifdef USE_VALUE_SUMMARY
    /* Following is required since C++ can't handle direct conversion from int/bool */
    PAny(int v) noexcept:
        type(&typeid(Int)), 
        i(v) 
    { }

    PAny(bool v) noexcept:
        type(&typeid(Bool)),
        b(v) 
    { }

    PAny(PMachine* v) noexcept:
        type(&typeid(Ptr<PMachine>)),
        m(v)
    { }
#endif

    inline operator const Int&() const {
        IF_ONLY(type != &typeid(Int)) {
            throw bad_cast();
        } else {
            return i;
        }
    }

    inline operator const Bool&() const {
        IF_ONLY(type != &typeid(Bool)) {
            throw bad_cast();
        } else {
            return b;
        }
    }

    inline operator Ptr<PMachine> const &() const {
        IF_ONLY(type != &typeid(Ptr<PMachine>)) {
            throw bad_cast();
        } else {
            return m;
        }
    }

    template<typename TO>
    using CastFunctionPointer = TO (*)(const PAny&);
    template<typename TO>
    using CastTableType = std::map<const type_info*, CastFunctionPointer<TO>>;
    template<typename, typename> struct CastJumpTable;
    template<typename ...FROMs, typename TO>
    struct CastJumpTable<List<FROMs...>, TO> {
        static CastTableType<TO>& table() {
            static CastTableType<TO> _table = { {&typeid(FROMs), &PAny::CastHelper<FROMs, TO>} ... };
            return _table;
        }
    };

    template<template<typename ...> class Container, typename ...Ts>
    operator Container<Ts...> () const {
#ifdef USE_VALUE_SUMMARY
        return unaryOp<Container<Ts...>>(type, [&](const type_info* type) {
            return CastJumpTable<DECL_TYPES, Container<Ts...>>::table().at(type)(*this);
        });
#else
        return CastJumpTable<DECL_TYPES, Container<Ts...>>::table().at(type)(*this);
#endif
    }


    using EqFunctionPointer = Bool (*)(const PAny&, const PAny&);
    using EqTableKeyType = std::pair<const type_info*, const type_info*>;
    using EqTableType = std::map<EqTableKeyType, EqFunctionPointer>;
    template<typename> struct EqJumpTable;
    template<typename ...Pair>
    struct EqJumpTable<List<Pair...>> {
        template<typename> struct Extract;
        template<typename A, typename B>
        struct Extract<List<A, B>> {
            using a = A;
            using b = B;
        };
        static EqTableType& table() {
            static EqTableType _table = 
                {   
                    { 
                        {
                            &typeid(typename Extract<Pair>::a), 
                            &typeid(typename Extract<Pair>::b) 
                        }, 
                        &PAny::EqHelperFunctor<typename Extract<Pair>::a, typename Extract<Pair>::b>::impl
                    } 
            ... };
            return _table;
        }
    };

    Bool operator == (const PAny& other) const {
#ifdef USE_VALUE_SUMMARY
        return binaryOp<Bool>(type, other.type, [&](const type_info* a, const type_info* b) {
            return EqJumpTable<product<List, DECL_TYPES, DECL_TYPES>::type>::table()
                    .at(make_pair(a, b))(*this, other);
        });
#else
        return EqJumpTable<product<List, DECL_TYPES, DECL_TYPES>::type>::table().at(make_pair(type, other.type))(*this, other);
#endif
    }

    Bool operator != (const PAny& other) const {
        return !(*this == other);
    }

    Ptr<const type_info> type;
#ifdef USE_VALUE_SUMMARY
    Int i = Int::undefined();
    Bool b = Bool::undefined();
    Ptr<PMachine> m = Ptr<PMachine>::undefined();
    AnyDataPointer ptr = AnyDataPointer::undefined();
#else
    Int i;
    Bool b;
    Ptr<PMachine> m;
    AnyDataPointer ptr;
#endif

#ifdef USE_VALUE_SUMMARY
    class Builder {
        Ptr<const type_info>::Builder type;
        Int::Builder i;
        Bool::Builder b;
        Ptr<PMachine>::Builder m;
        AnyDataPointer::Builder ptr;
    public:
        inline Builder& addValue(const Bdd& pred, const PAny& other) {
            type.addValue(pred, other.type);
            i.addValue(pred, other.i);
            b.addValue(pred, other.b);
            m.addValue(pred, other.m);
            ptr.addValue(pred, other.ptr);
            return *this;
        }

        inline PAny build() {
            return PAny(type.build(), i.build(), b.build(), m.build(), ptr.build());
        }
    };
#endif

    static const PAny& Null() {
        static const PAny _Null;
        return _Null;
    }

private:
    PAny(Ptr<const type_info>&& type, Int&& i, Bool&& b, Ptr<PMachine>&& m, AnyDataPointer&& ptr):
        type(std::move(type)),
        i(std::move(i)),
        b(std::move(b)),
        m(std::move(m)),
        ptr(std::move(ptr))
    { }

};

template<> 
struct PAny::EqHelperFunctor<Int, Int> {
    static Bool impl(const PAny& a, const PAny& b) {
        return a.i == b.i;
    }
};

template<> 
struct PAny::EqHelperFunctor<Bool, Bool> {
    static Bool impl(const PAny& a, const PAny& b) {
        return a.b == b.b;
    }
};

template<> 
struct PAny::EqHelperFunctor<Ptr<PMachine>, Ptr<PMachine>> {
    static Bool impl(const PAny& a, const PAny& b) {
        return a.m == b.m;
    }
};

template<typename A, typename B> 
struct PAny::EqHelperFunctor<A, B, typename std::enable_if<!IsCastable<A, B>::value>::type> {
    static Bool impl(const PAny& a, const PAny& b) {
        return false;
    }
};

template<typename A, typename B>
struct PAny::EqHelperFunctor<A, B, typename std::enable_if<IsCastable<A, B>::value && !IsSimpleType<A>::value>::type> {
    static Bool impl(const PAny& a, const PAny& b) {
        return get<A>(a.ptr) == getCast<B, A>(b.ptr);
    }
};

};

#endif