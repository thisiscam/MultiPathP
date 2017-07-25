#ifndef OPERATIONS_H
#define OPERATIONS_H

#ifdef USE_VALUE_SUMMARY
#include "ValueSummaryOperations.h"
#endif

namespace RUNTIME_NAMESPACE {

#ifdef USE_VALUE_SUMMARY

template<typename ReturnType, typename A, typename UnaryOp>
inline ReturnType unaryOp(const ValueSummary<A>& a, UnaryOp&& uOp) {
    return UnaryOpFunctor<ReturnType, A, UnaryOp>::impl(a, std::forward<UnaryOp>(uOp));
}

template<typename ReturnType, typename A, typename B, typename BinOp>
inline ReturnType binaryOp(const ValueSummary<A>& a, const ValueSummary<B>& b, BinOp&& binOp) {
    return BinaryOpFunctor<ReturnType, A, B, BinOp>::impl(a, b, std::forward<BinOp>(binOp));
}

#else

template<typename ReturnType, typename A, typename UnaryOp>
inline ReturnType unaryOp(A&& a, UnaryOp&& uOp) {
    return uOp(std::forward<A>(a));
}

template<typename ReturnType, typename A, typename B, typename BinOp>
inline ReturnType binaryOp(A&& a, B&& b, BinOp&& binOp) {
	return binOp(std::forward<A>(a), std::forward<B>(b));
}

#endif

}

#endif