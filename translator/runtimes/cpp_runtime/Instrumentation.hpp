#ifndef INTRUMENTATION_HPP
#define INTRUMENTATION_HPP

#include "PathConstraint.hpp"
#include "ValueSummary.hpp"

#include "FunctionInstrumentationHelper.h"

#define DECL_RET(ReturnType) ReturnType __ret
#define DECL_RET_IF_NOT_VOID(ReturnType) IF_TYPE_MATCH_VOID( ReturnType , void __ret() , DECL_RET(ReturnType) )

#define RET_IF_NOT_VOID(ReturnType) IF_TYPE_MATCH_VOID( ReturnType , , return __ret; )

#define FUNCTION_BODY(body)                                 \
    Bdd __oldPc = PathConstraint::pc();                     \
    auto __body = [&](){                                    \
        body;                                               \
    };                                                      \
    __body();                                               \
    PathConstraint::pc() = __oldPc;                         \

#define FUNCTION_DECL(ReturnType, functionName, args, body) \
ReturnType functionName args {                              \
    DECL_RET_IF_NOT_VOID(ReturnType);                       \
    FUNCTION_BODY(body)                                     \
    RET_IF_NOT_VOID(ReturnType);                            \
}

#define IF(condition)               \
    {                               \
        if(IfBranch::eval(condition

#define THEN(block)                 \
        ,                           \
        [&](){                      \
            block;                  \
        }

#define ELSE(block)                 \
        ,                           \
        [&](){                      \
            block;                  \
        }

#define ENDIF()                     \
        )) {                        \
            return;                 \
        }                           \
    }

#define WHILE(condition, block)                 \
    {                                           \
        Bdd loopMergePointPc;                   \
        if(LoopBranch::eval(                    \
        [&] { return condition; },              \
        [&] {                                   \
            block                               \
        }, loopMergePointPc)) {                 \
            return;                             \
        }                                       \
    }

#define BREAK()                                     \
    {                                               \
        loopMergePointPc |= PathConstraint::pc();   \
        PathConstraint::pc() = Bdd::bddZero();      \
        return;                                     \
    }

#define RETURN(val)                             \
    {                                           \
        __ret = (val);                          \
        PathConstraint::pc() = Bdd::bddZero();  \
        return;                                 \
    }

namespace RUNTIME_NAMESPACE {

static inline bool merge() {
    return PathConstraint::isZero();
}

class IfBranch final {

public:

    template<typename Then, typename Else>
    static inline bool eval(ValueSummary<bool> condition, Then&& thenBlock, Else&& elseBlock) {
        Bdd falseBranch = condition.F;
        PathConstraint::pc() &= condition.T;
        if(PathConstraint::isNotZero()) {
            thenBlock();
        }
        Bdd mergePointPc = PathConstraint::pc();
        PathConstraint::pc() = falseBranch;
        if(PathConstraint::isNotZero()) {
            elseBlock();
        }
        PathConstraint::pc() |= mergePointPc;
        return merge();
    }

    template<typename Then>
    static inline bool eval(ValueSummary<bool> condition, Then&& thenBlock) {
        Bdd falseBranch = condition.F;
        PathConstraint::pc() &= condition.T;
        if(PathConstraint::isNotZero()) {
            thenBlock();
        }
        PathConstraint::pc() |= falseBranch;
        return merge();
    }
};


class LoopBranch final {

public:

    template<typename Condition, typename Body>
    static inline bool eval(Condition&& conditionExp, Body&& block, Bdd& loopMergePointPc) {
        while(true) {
            ValueSummary<bool> condition = conditionExp();
            loopMergePointPc |= condition.F;
            if(condition.T.isZero()) {
                break;
            } else {
                PathConstraint::pc() = condition.T;
                block();
                if(PathConstraint::isZero()) {
                    break;
                }
            }
        }
        PathConstraint::pc() |= loopMergePointPc;
        return merge();
    }

};

};
#endif