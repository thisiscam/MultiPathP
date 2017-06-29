#ifndef INTRUMENTATION_HPP
#define INTRUMENTATION_HPP


/* WARNING: the following macro is not portable */
#define GET_MACRO(_0, _1, NAME, ...) NAME
#define RETURN(...) GET_MACRO(_0, ##__VA_ARGS__, RETURN1, RETURN0)(__VA_ARGS__)

#ifdef USE_VALUE_SUMMARY

#include "PathConstraint.hpp"
#include "ValueSummary.hpp"


#define BEGIN_BLOCK()                                           \
    do {

#define END_BLOCK()                                             \
    } while(false);

#define FUNCTION_DECL(ReturnType, functionName, args)           \
ReturnType functionName args {                                  \
    typename ReturnType::Builder __ret;                         \
    Bdd __oldPc = PathConstraint::pc();                         \
    BEGIN_BLOCK()

#define END_FUNCTION()                                          \
    END_BLOCK()                                                 \
    PathConstraint::pc() = __oldPc;                             \
    return __ret.build();                                       \
}

#define VOID_FUNCTION_DECL(functionName, args)                  \
void functionName args {                                        \
    Bdd __oldPc = PathConstraint::pc();                         \
    BEGIN_BLOCK()

#define END_VOID_FUNCTION()                                     \
    END_BLOCK()                                                 \
    PathConstraint::pc() = __oldPc;                             \
}

#define IF(_condition...)                                       \
    {                                                           \
        Bool&& condition = (_condition);                        \
        Bdd&& falseBranch = PathConstraint::pc() & condition.F; \
        Bdd mergePointPc = falseBranch;

#define THEN()                                                  \
        PathConstraint::pc() &= condition.T;                    \
        if(PathConstraint::isNotZero()) {                       \
            BEGIN_BLOCK();

#define ELSE()                                                  \
            END_BLOCK();                                        \
        }                                                       \
        mergePointPc = PathConstraint::pc();                    \
        PathConstraint::pc() = falseBranch;                     \
        if(PathConstraint::isNotZero()) {                       \
            BEGIN_BLOCK();

#define ENDIF()                                                 \
            END_BLOCK();                                        \
        }                                                       \
        PathConstraint::pc() |= mergePointPc;                   \
        if(PathConstraint::isZero()) {                          \
            break;                                              \
        }                                                       \
    }

#define IF_ONLY(condition...)                                   \
    if(((PathConstraint::pc() & (condition).F).isZero()))       \


#define WHILE(_condition...)                                    \
    {                                                           \
        Bdd loopMergePointPc;                                   \
        while(true) {                                           \
            Bool&& condition = (_condition);                    \
            loopMergePointPc |= condition.F;                    \
            if(condition.T.isZero()) {                          \
                break;                                          \
            } else {                                            \
                PathConstraint::pc() = condition.T;             \
                BEGIN_BLOCK();
        

#define ENDWHILE()                                              \
                END_BLOCK();                                    \
                if(PathConstraint::isZero()) {                  \
                    break;                                      \
                }                                               \
            }                                                   \
        }                                                       \
        PathConstraint::pc() |= loopMergePointPc;               \
        if(PathConstraint::isZero()) {                          \
            break;                                              \
        }                                                       \
    }

#define ENDWHILE_NC()                                           \
                END_BLOCK();                                    \
                if(PathConstraint::isZero()) {                  \
                    break;                                      \
                }                                               \
            }                                                   \
        }                                                       \
        PathConstraint::pc() |= loopMergePointPc;               \
    }

#define FOR(initializer, condition, increment, block...)        \
    {                                                           \
        initializer;                                            \
        WHILE(condition) {                                      \
            block;                                              \
            increment;                                          \
        }

#define ENDFOR()                                                \
        ENDWHILE()                                              \
    }

#define ENDFOR_NC()                                             \
        ENDWHILE_NC()                                           \
    }


#define BREAK()                                                 \
    {                                                           \
        loopMergePointPc |= PathConstraint::pc();               \
        PathConstraint::pc() = Bdd::bddZero();                  \
        break;                                                  \
    }

#define RETURN0()                                               \
    {                                                           \
        PathConstraint::pc() = Bdd::bddZero();                  \
        break;                                                  \
    }

#define RETURN1(val)                                            \
    {                                                           \
        __ret.addValue(PathConstraint::pc(), std::move(val));   \
        PathConstraint::pc() = Bdd::bddZero();                  \
        break;                                                  \
    }


#else

#define BEGIN_BLOCK()   {
#define END_BLOCK()     }

#define FUNCTION_DECL(ReturnType, functionName, args)           \
ReturnType functionName args {                                  \

#define END_FUNCTION()                                          \
}

#define VOID_FUNCTION_DECL(functionName, args)                  \
void functionName args {


#define END_VOID_FUNCTION()                                     \
}

#define IF(condition...)                                        \
    if(condition)

#define THEN()                                                  \
    {

#define ELSE()                                                  \
    } else {   

#define ENDIF()                                                 \
    }

#define IF_ONLY(condition...)                                   \
    if(condition)          

#define WHILE(condition...)                                     \
    while(condition) {

#define ENDWHILE()                                              \
    }

#define ENDWHILE_NC()                                           \
    }

#define FOR(initializer, condition, increment)                  \
    for(initializer; condition; increment)  {                   \

#define ENDFOR()                                                \
    }

#define ENDFOR_NC()                                             \
    }

#define BREAK() break;

#define RETURN0() return;

#define RETURN1(val) return val;

#endif
#endif