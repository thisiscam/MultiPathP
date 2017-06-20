#ifndef VALUE_SUMMARY_HPP
#define VALUE_SUMMARY_HPP

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
	Bdd predicate;
	T value;

	Guard(const Bdd& predicate, const T& value):
		predicate(predicate),
		value(value) 
	{ }
};

template<typename ReturnType, typename A, typename B, typename BinOp>
ValueSummary<ReturnType>
binaryOp(const ValueSummary<A>& a, const ValueSummary<B>& b, BinOp&& binOp) {
	typename ValueSummary<ReturnType>::Builder builder;
	for(const auto& gvA : a.values) {
		for(const auto& gvB : b.values) {
			Bdd pred = PathConstraint::pc() & gvA.second & gvB.second;
			if(!pred.isZero()) {
				builder.addValue(pred, binOp(gvA.first, gvB.first));
			}
		}
	}
	return builder.build();
}

template<typename ReturnType, typename A, typename UnaryOp>
ValueSummary<ReturnType> unaryOp(const ValueSummary<A>& a, UnaryOp&& uOp) {
	typename ValueSummary<ReturnType>::Builder builder;
	for(const auto& gv : a.values) {
		Bdd pred = PathConstraint::pc() & gv.second;
		if(!pred.isZero()) {
			builder.addValue(pred, uOp(gv.first));
		}
	}
	return builder.build();
}

};

#include "VS.hpp"
#include "VSBool.hpp"
#include "VSInt.hpp"
#include "VSPtr.hpp"

#endif