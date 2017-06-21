#ifndef INTRUMENTATION_HPP
#define INTRUMENTATION_HPP

#ifdef USE_VALUE_SUMMARY

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

#define IF(condition)                           \
    {                                           \
        bool terminated =                       \
            IfBranch::eval(condition            \

#define THEN(block)                             \
        ,                                       \
        [&](){                                  \
            block;                              \
        }

#define ELSE(block)                             \
        ,                                       \
        [&](){                                  \
            block;                              \
        }

#define ENDIF()                                 \
        );                                      \
        if(terminated) {                        \
            return;                             \
        }                                       \
    }

#define ENDIF_NC()                              \
        );                                      \
    }

#define IF_ONLY(condition)                      \
    if(IfBranch::onlyTrue(condition))          


#define WHILE(condition, block)                 \
    {                                           \
        Bdd loopMergePointPc;                   \
        bool terminated =                       \
            LoopBranch::eval(                   \
        [&] { return condition; },              \
        [&] {                                   \
            block                               \
        }, loopMergePointPc);

#define ENDWHILE()                              \
        if(terminated) {                        \
            return;                             \
        }                                       \
    }

#define ENDWHILE_NC()                            \
    }

#define FOR(initializer, condition, increment, block)   \
    {                                                   \
        initializer;                                    \
        WHILE(condition, {                              \
            block;                                      \
            increment;                                  \
        })

#define ENDFOR()                                        \
        ENDWHILE()                                      \
    }

#define ENDFOR_NC()                                     \
        ENDWHILE_NC()                                   \
    }

#define BREAK()                                     \
    {                                               \
        loopMergePointPc |= PathConstraint::pc();   \
        PathConstraint::pc() = Bdd::bddZero();      \
        return;                                     \
    }

/* WARNING: the following macro is not portable */
#define GET_MACRO(_0, _1, NAME, ...) NAME
#define RETURN(...) GET_MACRO(_0, ##__VA_ARGS__, RETURN1, RETURN0)(__VA_ARGS__)

#define RETURN0()                               \
    {                                           \
        PathConstraint::pc() = Bdd::bddZero();  \
        return;                                 \
    }

#define RETURN1(val)                            \
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
    static inline bool eval(const ValueSummary<bool>& condition, Then&& thenBlock, Else&& elseBlock) {
        Bdd falseBranch = PathConstraint::pc() & condition.F;
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
    static inline bool eval(const ValueSummary<bool>& condition, Then&& thenBlock) {
        Bdd falseBranch = PathConstraint::pc() & condition.F;
        PathConstraint::pc() &= condition.T;
        if(PathConstraint::isNotZero()) {
            thenBlock();
        }
        PathConstraint::pc() |= falseBranch;
        return merge();
    }

    static inline bool onlyTrue(const ValueSummary<bool>& condition) {
        Bdd falseBranch = PathConstraint::pc() & condition.F;
        return falseBranch.isZero();
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

#else

#define FUNCTION_DECL(ReturnType, functionName, args, body) \
ReturnType functionName args {                              \
    body;                                                   \
}

#define IF(condition)               \
    if(condition)                   \

#define THEN(block)                 \
    {                               \
        block;                      \
    }

#define ELSE(block)                 \
    {                               \
        block;                      \
    }

#define ENDIF()

#define IF_ONLY(condition)          \
    if(condition)          

#define WHILE(condition, block)     \
    while(condition) {              \
        block;                      \
    }

#define FOR(initializer, condition, increment, block)   \
    for(initializer; condition; increment) {            \
        block;                                          \
    }

#define BREAK() break;

/* WARNING: the following macro is not portable */
#define GET_MACRO(_0, _1, NAME, ...) NAME
#define RETURN(...) GET_MACRO(_0, ##__VA_ARGS__, RETURN1, RETURN0)(__VA_ARGS__)

#define RETURN0() return;

#define RETURN1(val) return val;

#endif
#endif