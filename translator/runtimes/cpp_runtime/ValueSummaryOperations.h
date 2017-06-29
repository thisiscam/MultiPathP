#ifndef VALUE_SUMMARY_OPERATIONS_HPP
#define VALUE_SUMMARY_OPERATIONS_HPP

#include <utility>
namespace RUNTIME_NAMESPACE {

template<typename ReturnType, typename A, typename B, typename BinOp>
struct BinaryOpFunctor;


template<typename A, typename B, typename BinOp>
struct BinaryOpFunctor<void, A, B, BinOp> {
    inline static void 
    impl(const ValueSummary<A>& a, const ValueSummary<B>& b, BinOp&& binOp) {
        for(const auto& gvA : a.values) {
            for(const auto& gvB : b.values) {            
                Bdd oldPc = PathConstraint::pc();
                PathConstraint::pc() &= gvA.second & gvB.second;
                if(PathConstraint::isNotZero()) {
                    binOp(gvA.first, gvB.first);
                }
                PathConstraint::pc() = oldPc;
            }
        }
    }
};


template<typename ReturnType, typename A, typename B, typename BinOp>
struct BinaryOpFunctor {
    static ReturnType
    impl(const ValueSummary<A>& a, const ValueSummary<B>& b, BinOp&& binOp) {
        typename ReturnType::Builder builder;
        for(const auto& gvA : a.values) {
            for(const auto& gvB : b.values) {
                Bdd oldPc = PathConstraint::pc();
                PathConstraint::pc() &= gvA.second & gvB.second;
                if(PathConstraint::isNotZero()) {
                    builder.addValue(PathConstraint::pc(), binOp(gvA.first, gvB.first));
                }
                PathConstraint::pc() = oldPc;
            }
        }
        return builder.build();
    }
};

template<typename ReturnType, typename A, typename B, typename BinOp>
inline ReturnType binaryOp(const ValueSummary<A>& a, const ValueSummary<B>& b, BinOp&& binOp) {
    return BinaryOpFunctor<ReturnType, A, B, BinOp>::impl(a, b, std::forward<BinOp>(binOp));
}

template<typename ReturnType, typename A, typename UnaryOp>
struct UnaryOpFunctor;

template<typename A, typename UnaryOp>
struct UnaryOpFunctor<void, A, UnaryOp> {
    static void
    impl(const ValueSummary<A>& a, UnaryOp&& uOp) {
        for(const auto& gvA : a.values) {
            Bdd oldPc = PathConstraint::pc();
            PathConstraint::pc() &= gvA.second;
            if(PathConstraint::isNotZero()) {
                uOp(gvA.first);
            }
            PathConstraint::pc() = oldPc;
        }
    }
};

template<typename ReturnType, typename A, typename UnaryOp>
struct UnaryOpFunctor {
    static ReturnType
    impl(const ValueSummary<A>& a, UnaryOp&& uOp) {
        typename ReturnType::Builder builder;
        for(const auto& gvA : a.values) {
            Bdd oldPc = PathConstraint::pc();
            PathConstraint::pc() &= gvA.second;
            if(PathConstraint::isNotZero()) {
                builder.addValue(PathConstraint::pc(), uOp(gvA.first));
            }
            PathConstraint::pc() = oldPc;
        }
        return builder.build();
    }
};


template<typename ReturnType, typename A, typename UnaryOp>
inline ReturnType unaryOp(const ValueSummary<A>& a, UnaryOp&& uOp) {
    return UnaryOpFunctor<ReturnType, A, UnaryOp>::impl(a, std::forward<UnaryOp>(uOp));
}

};
#endif