#ifndef PANY_HPP
#define PANY_HPP

#include <memory>
#include <stdexcept>
#include <typeinfo>
#include <map>
#include <iostream>
#include <utility>

#include "PTypePtr.h"
#include "PTypeBoxed.hpp"
#include "Operations.h"
#include "utils/crossproduct.hpp"
#include "utils/permute.hpp"
#include "utils/type_helper.h"

using namespace std;

namespace RUNTIME_NAMESPACE {

#ifdef USE_VALUE_SUMMARY
using PAnyVSMapType = std::unordered_map<const type_info*, std::pair<std::shared_ptr<PTypePtr>, Bdd>>;

template<typename ReturnType, typename UnaryOp>
static inline ReturnType unaryOp(const PAnyVSMapType& a, UnaryOp&& uOp) {
    Bdd oldPc = PathConstraint::pc();
    typename ReturnType::Builder builder;
    for(const auto& gvA : a) {
        PathConstraint::pc() = oldPc & gvA.second.second;
        if(PathConstraint::isNotZero()) {
            builder.addValue(PathConstraint::pc(), uOp(gvA.second.first));
        }
    }
    PathConstraint::pc() = oldPc;
    return builder.build();
}

template<typename ReturnType, typename BinOp>
static inline ReturnType binaryOp(const PAnyVSMapType& a, const PAnyVSMapType& b, BinOp&& binOp) {
    typename ReturnType::Builder builder;
    Bdd oldPc = PathConstraint::pc();
    for(const auto& gvA : a) {
        Bdd&& predA = oldPc & gvA.second.second;
        if(!predA.isZero()) {
            for(const auto& gvB : b) {
                PathConstraint::pc() = predA & gvB.second.second;
                if(PathConstraint::isNotZero()) {
                    builder.addValue(PathConstraint::pc(), binOp(gvA.second.first, gvB.second.first));
                }
            }
        }
    }
    PathConstraint::pc() = oldPc;
    return builder.build();
}
#endif

template<typename T>
struct IsSimpleType
{
    static constexpr bool value = std::is_same<T, int>::value || std::is_same<T, bool>::value || std::is_same<T, PMachine*>::value;
};

template <template <typename...> class, template<typename...> class> 
struct is_same_template : std::false_type{};

template <template <typename...> class T>
struct is_same_template<T,T> : std::true_type{};

template<typename>
struct IsContrainerType
{
    static constexpr bool value = false;
};

template<template<typename ...Ts> class Container, typename ...Ts>
struct IsContrainerType<Container<Ts...>>
{
    static constexpr bool value = 
        is_same_template<Container, PList>::value
     || is_same_template<Container, PTuple>::value
     || is_same_template<Container, PMap>::value
    ;
};

template<typename T>
struct IsBoxedSimpleType
{
    static constexpr bool value = false;
};

template<typename T>
struct IsBoxedSimpleType<PTypeBoxed<T>>
{
    static constexpr bool value = IsSimpleType<T>::value;
};

#ifdef USE_VALUE_SUMMARY
template<typename T> 
struct IsVSSimpleType {
    static constexpr bool value = false;
};

template<typename T>
struct IsVSSimpleType<ValueSummary<T>>
{
    static constexpr bool value = IsSimpleType<T>::value;
};
#endif

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

template<typename> struct MapBoxed;

template<template <typename ...Ts> class Container>
struct MapBoxed<Container<>> {
    using type = Container<>;
};

template<template <typename ...Ts> class Container, typename Head, typename ...Rest>
struct MapBoxed<Container<Head, Rest...>> {
    template<typename T>
#ifdef USE_VALUE_SUMMARY
    using IsBasicType = IsVSSimpleType<T>;
#else
    using IsBasicType = IsSimpleType<T>;
#endif
    using type = 
        typename Merge<
            Container<typename std::conditional<IsBasicType<Head>::value, PTypeBoxed<Head>, Head>::type>,
            typename MapBoxed<Container<Rest...>>::type
        >::type;
};

using ProcessedDeclTypes = typename MapBoxed<DECL_TYPES>::type;

class PAny final {

private:

    template<typename, typename, typename=void>
    struct CastHelperFunctor;

    template<typename, typename, typename=void>
    struct EqHelperFunctor;

public:

    PAny() = default;

    PAny(const PAny&) = default; //TODO: why doesn't this require filtering?

    PAny(PAny&& other) = default;

#ifdef USE_VALUE_SUMMARY
    PAny& operator=(const PAny& rhs) {
        Bdd oldPc = PathConstraint::pc();
        PAnyVSMapType tmpRhsMap;
        for(const auto& gvRhs : rhs.ptr) {
            PathConstraint::pc() = oldPc & gvRhs.second.second;
            if(PathConstraint::isNotZero()) {
                bool found = false;
                for(auto gvLhs = begin(ptr); gvLhs != end(ptr); ) {
                    if(gvLhs->first == gvRhs.first) {
                        gvLhs->second.second |= PathConstraint::pc();
                        if(gvLhs->second.first.use_count() > 1) {
                            gvLhs->second.first = std::shared_ptr<PTypePtr>(gvRhs.second.first->clone());
                        }
                        gvLhs->second.first->assign(*gvRhs.second.first.get());
                        found = true;
                        ++gvLhs;
                    } else {
                        gvLhs->second.second &= !PathConstraint::pc();
                        if (gvLhs->second.second.isZero()) {
                            gvLhs = ptr.erase(gvLhs);
                        } else {
                            ++gvLhs;
                        }
                    }
                }
                if(!found) {
                    tmpRhsMap.insert({gvRhs.first, {gvRhs.second.first, PathConstraint::pc()}});
                }
            }
        }
        ptr.insert(tmpRhsMap.begin(), tmpRhsMap.end());
        PathConstraint::pc() = oldPc;
        return *this;
    }
#else
    PAny& operator=(const PAny& rhs) = default;
#endif

#ifdef USE_VALUE_SUMMARY

    PAny(const Int& i) noexcept:
        ptr({{&typeid(PTypeBoxed<Int>), {std::make_shared<PTypeBoxed<Int>>(i), PathConstraint::pc()}}})
    {
    }

    PAny(const Bool& b) noexcept:
        ptr({{&typeid(PTypeBoxed<Bool>), {std::make_shared<PTypeBoxed<Bool>>(b), PathConstraint::pc()}}})
    { }

    PAny(const Ptr<PMachine>& m) noexcept:
        ptr({{&typeid(PTypeBoxed<Ptr<PMachine>>), {std::make_shared<PTypeBoxed<Ptr<PMachine>>>(m), PathConstraint::pc()}}})
    { }

    PAny(const int i) noexcept:
        PAny(Int(i))
    { }

    PAny(const bool b) noexcept:
        PAny(Bool(b))
    { }

    PAny(PMachine* m) noexcept:
        PAny(Ptr<PMachine>(m))
    { }

    template<
        typename ContainerType, 
        typename=typename std::enable_if<IsContrainerType<typename std::remove_reference<ContainerType>::type>::value>::type
    >
    PAny(ContainerType&& v) noexcept:
        ptr(
        {{
            &typeid(ContainerType), 
            {
                std::make_shared<
                    typename std::remove_reference<typename std::remove_cv<ContainerType>::type>::type
                >(std::forward<ContainerType>(v)),
                PathConstraint::pc()
            }
        }}
        )
    { }
#else
    template<typename FromType, typename=typename std::enable_if<!IsSimpleType<FromType>::value>::type>
    PAny(FromType&& v) noexcept:
        ptr(std::make_shared<typename std::remove_reference<typename std::remove_cv<FromType>::type>::type>(std::forward<FromType>(v)))
    { }

    PAny(int i) noexcept:
        ptr(std::make_shared<PTypeBoxed<Int>>(i)) 
    { }

    PAny(bool b) noexcept:
        ptr(std::make_shared<PTypeBoxed<Bool>>(b)) 
    { }

    PAny(Ptr<PMachine> m) noexcept:
        ptr(std::make_shared<PTypeBoxed<Ptr<PMachine>>>(m))
    { }
#endif

    template<typename TO>
    using CastFunctionPointer = TO (*)(const std::shared_ptr<PTypePtr>&);
    template<typename TO>
    using CastTableType = std::map<const type_info*, CastFunctionPointer<TO>>;
    template<typename, typename> struct CastJumpTable;
    template<typename ...FROMs, typename TO>
    struct CastJumpTable<List<FROMs...>, TO> {
        static CastTableType<TO>& table() {
            static CastTableType<TO> _table = { {&typeid(FROMs), &PAny::CastHelperFunctor<FROMs, TO>::impl} ... };
            return _table;
        }
    };

    template<typename CastToType>
    operator CastToType () const {
        return unaryOp<CastToType>(ptr, [&](const std::shared_ptr<PTypePtr>& ptr) {
            const PTypePtr* _ptr = ptr.get();
            return CastJumpTable<ProcessedDeclTypes, CastToType>::table().at(&typeid(*_ptr))(ptr);
        });
    }

    operator Int() const {
        return unaryOp<Int>(ptr, [&](const std::shared_ptr<PTypePtr>& ptr) {
            const PTypePtr* _ptr = ptr.get();
            return CastJumpTable<ProcessedDeclTypes, PTypeBoxed<Int>>::table().at(&typeid(*_ptr))(ptr).value;
        });
    }

    operator Bool() const {
        return unaryOp<Bool>(ptr, [&](const std::shared_ptr<PTypePtr>& ptr) {
            const PTypePtr* _ptr = ptr.get();
            return CastJumpTable<ProcessedDeclTypes, PTypeBoxed<Bool>>::table().at(&typeid(*_ptr))(ptr).value;
        });
    }

    operator Ptr<PMachine>() const {
        return unaryOp<Ptr<PMachine>>(ptr, [&](const std::shared_ptr<PTypePtr>& ptr) {
            const PTypePtr* _ptr = ptr.get();
            const auto& xxx = dynamic_cast<const PTypeBoxed<Ptr<PMachine>>*>(_ptr);
            const auto& t = &typeid(*_ptr);
            auto&& x = CastJumpTable<ProcessedDeclTypes, PTypeBoxed<Ptr<PMachine>>>::table().at(&typeid(*_ptr))(ptr).value;
            return x;
        });
    }

    using EqFunctionPointer = Bool (*)(const std::shared_ptr<PTypePtr>&, const std::shared_ptr<PTypePtr>&);
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

    Bool operator ==(const PAny& other) const {
        return binaryOp<Bool>(ptr, other.ptr, [&](const std::shared_ptr<PTypePtr>& a, const std::shared_ptr<PTypePtr>& b) {
            const PTypePtr* _a = a.get();
            const PTypePtr* _b = b.get();
            return EqJumpTable<product<List, ProcessedDeclTypes, ProcessedDeclTypes>::type>::table()
                    .at(make_pair(&typeid(*_a), &typeid(*_b)))(a, b);
        });
    }

    Bool operator !=(const PAny& other) const {
        return binaryOp<Bool>(ptr, other.ptr, [&](const std::shared_ptr<PTypePtr>& a, const std::shared_ptr<PTypePtr>& b) {
            const PTypePtr* _a = a.get();
            const PTypePtr* _b = b.get();
            return !EqJumpTable<product<List, ProcessedDeclTypes, ProcessedDeclTypes>::type>::table()
                    .at(make_pair(&typeid(*_a), &typeid(*_b)))(a, b);
        });
    }

#ifdef USE_VALUE_SUMMARY
    PAnyVSMapType ptr;

    class Builder {
    public:
        inline void addValue(const Bdd& pred, const std::shared_ptr<PTypePtr>& value) {
            const PTypePtr* _value = value.get();
            auto& entry = ptr[&typeid(*_value)];
            Bdd oldPc = PathConstraint::pc();
            PathConstraint::pc() = pred;
            if(entry.first) {
                if(entry.first.use_count() > 1) {
                    entry.first = std::shared_ptr<PTypePtr>(_value->clone());
                }
                entry.first->assign(*_value);
            } else {
                entry.first = std::shared_ptr<PTypePtr>(value->clone());
            }
            PathConstraint::pc() = oldPc;
            entry.second |= pred;
        }

        inline void addValue(const Bdd& pred, const PAny& rhs) {
            for(const auto& v : rhs.ptr) {
                Bdd&& constraintedPred = pred & v.second.second;
                if(!constraintedPred.isZero()) {
                    addValue(constraintedPred, v.second.first);
                }
            }
        }

        inline PAny build() {
            return PAny(std::move(ptr));
        }

        PAnyVSMapType ptr;
    };

private:
    PAny(PAnyVSMapType&& ptr):
        ptr(std::move(ptr))
    { }
public:
#else
    std::shared_ptr<PTypePtr> ptr;
#endif

    static const PAny& Null() {
        static const PAny _Null;
        return _Null;
    }
};

template<typename FROM, typename TO>
struct PAny::CastHelperFunctor<FROM, TO, typename std::enable_if<!IsCastable<FROM, TO>::value>::type> {
    static TO impl(const std::shared_ptr<PTypePtr>& from) {
        throw bad_cast();
    }
};

template<typename FROM, typename TO>
struct PAny::CastHelperFunctor<FROM, TO, typename std::enable_if<IsCastable<FROM, TO>::value>::type> {
    static TO impl(const std::shared_ptr<PTypePtr>& fromPtr) {
        TO&& ret = std::forward<TO>(*dynamic_pointer_cast<FROM>(fromPtr)); 
        return ret;
    }
};

template<typename A, typename B> 
struct PAny::EqHelperFunctor<A, B, typename std::enable_if<!IsCastable<A, B>::value>::type> {
    static Bool impl(const std::shared_ptr<PTypePtr>& a, const std::shared_ptr<PTypePtr>& b) {
        return false;
    }
};

template<typename A, typename B>
struct PAny::EqHelperFunctor<A, B, typename std::enable_if<IsCastable<A, B>::value>::type> {
    static Bool impl(const std::shared_ptr<PTypePtr>& a, const std::shared_ptr<PTypePtr>& b) {
        return *dynamic_pointer_cast<A>(a) == std::forward<A>(*dynamic_pointer_cast<B>(b));
    }
};

};

#endif