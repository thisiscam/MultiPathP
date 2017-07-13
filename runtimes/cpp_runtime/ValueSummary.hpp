#ifndef VALUE_SUMMARY_HPP
#define VALUE_SUMMARY_HPP

#ifndef USE_VALUE_SUMMARY
#error "This fine should only be included with value summary"
#endif

#include <sylvan_obj.hpp>

namespace RUNTIME_NAMESPACE {

template<typename T, typename U>
struct IsConstLValueReferenceOfType {
    static constexpr bool value = 
        !std::is_same<
            T, 
            typename std::add_const<typename std::add_lvalue_reference<U>::type>::type
        >::value;
};

template<typename T, typename U>
struct IsConstRValueReferenceOfType {
    static constexpr bool value = 
        !std::is_same<
            T, 
            typename std::add_const<typename std::add_rvalue_reference<U>::type>::type
        >::value;
};

template<typename T, typename U>
struct IsConstReferenceOfType
{
    static constexpr bool value = 
        IsConstLValueReferenceOfType<T, U>::value || 
        IsConstRValueReferenceOfType<T, U>::value;
};


template<typename T>
class ValueSummary;


template<typename T>
struct Guard {
    Bdd second;
    T first;

    Guard(const Bdd& second, const T& first):
        first(first),
        second(second) 
    { }
};

};

#include "VSBool.hpp"
#include "VSInt.hpp"
#include "VS.hpp"

#endif