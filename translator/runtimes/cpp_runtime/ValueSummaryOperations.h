#ifndef VALUE_SUMMARY_OPERATIONS_HPP
#define VALUE_SUMMARY_OPERATIONS_HPP

#include <utility>
namespace RUNTIME_NAMESPACE {

template<typename ReturnType, bool constraintPc, typename A, typename B, typename BinOp>
struct BinaryOpFunctor;

template<typename ReturnType, bool constraintPc, typename A, typename B, typename BinOp>
struct BinaryOpFunctor {
    inline static ReturnType 
    impl(const ValueSummary<A>& a, const ValueSummary<B>& b, BinOp&& binOp) {
        ReturnType r;
        for(const auto& gvA : a.values) {
            for(const auto& gvB : b.values) {            
                Bdd oldPc = PathConstraint::pc();
                PathConstraint::pc() &= gvA.second & gvB.second;
                if(PathConstraint::isNotZero()) {
                    r = binOp(gvA.first, gvB.first);
                }
                PathConstraint::pc() = oldPc;
            }
        }
        return r;
    }
};

template<bool constraintPc, typename A, typename B, typename BinOp>
struct BinaryOpFunctor<void, constraintPc, A, B, BinOp> {
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


template<typename VSReturnType, bool constraintPc, typename A, typename B, typename BinOp>
struct BinaryOpFunctor<ValueSummary<VSReturnType>, constraintPc, A, B, BinOp> {
    static ValueSummary<VSReturnType>
    impl(const ValueSummary<A>& a, const ValueSummary<B>& b, BinOp&& binOp) {
        typename ValueSummary<VSReturnType>::Builder builder;
        for(const auto& gvA : a.values) {
            for(const auto& gvB : b.values) {
                if(constraintPc) {
                    Bdd oldPc = PathConstraint::pc();
                    PathConstraint::pc() &= gvA.second & gvB.second;
                    if(PathConstraint::isNotZero()) {
                        builder.addValue(PathConstraint::pc(), binOp(gvA.first, gvB.first));
                    }
                    PathConstraint::pc() = oldPc;
                } else {
                    Bdd pred = PathConstraint::pc() & gvA.second & gvB.second;
                    if(!pred.isZero()) {
                        builder.addValue(pred, binOp(gvA.first, gvB.first));
                    }
                }
            }
        }
        return builder.build();
    }
};

template<typename ReturnType, bool constraintPc = false, typename A, typename B, typename BinOp>
inline ReturnType binaryOp(const ValueSummary<A>& a, const ValueSummary<B>& b, BinOp&& binOp) {
    return BinaryOpFunctor<ReturnType, constraintPc, A, B, BinOp>::impl(a, b, std::forward<BinOp>(binOp));
}

template<typename ReturnType, bool constraintPc, typename A, typename UnaryOp>
struct UnaryOpFunctor;

template<typename ReturnType, bool constraintPc, typename A, typename UnaryOp>
struct UnaryOpFunctor {
    static ReturnType
    impl(const ValueSummary<A>& a, UnaryOp&& uOp) {
        ReturnType r;
        for(const auto& gvA : a.values) {
            Bdd oldPc = PathConstraint::pc();
            PathConstraint::pc() &= gvA.second;
            if(PathConstraint::isNotZero()) {
                r = uOp(gvA.first);
            }
            PathConstraint::pc() = oldPc;
        }
        return r;
    }
};

extern bool& blah();

template<bool constraintPc, typename A, typename UnaryOp>
struct UnaryOpFunctor<void, constraintPc, A, UnaryOp> {
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

template<typename VSReturnType, bool constraintPc, typename A, typename UnaryOp>
struct UnaryOpFunctor<ValueSummary<VSReturnType>, constraintPc, A, UnaryOp> {
    static ValueSummary<VSReturnType>
    impl(const ValueSummary<A>& a, UnaryOp&& uOp) {
        typename ValueSummary<VSReturnType>::Builder builder;
        for(const auto& gvA : a.values) {
            if(constraintPc) {
                Bdd oldPc = PathConstraint::pc();
                PathConstraint::pc() &= gvA.second;
                if(PathConstraint::isNotZero()) {
                    builder.addValue(PathConstraint::pc(), uOp(gvA.first));
                }
                PathConstraint::pc() = oldPc;
            } else {
                Bdd pred = PathConstraint::pc() & gvA.second;
                if(!pred.isZero()) {
                    builder.addValue(pred, uOp(gvA.first));
                }
            }
        }
        return builder.build();
    }
};


template<typename ReturnType, bool constraintPc = false, typename A, typename UnaryOp>
inline ReturnType unaryOp(const ValueSummary<A>& a, UnaryOp&& uOp) {
    return UnaryOpFunctor<ReturnType, constraintPc, A, UnaryOp>::impl(a, std::forward<UnaryOp>(uOp));
}

};
#endif