// // #define USE_VALUE_SUMMARY
// #include "header.h"

// using namespace pingpong;

// int main(int argc, char const *argv[])
// {
//  ExecutionEngine engine = ExecutionEngine();
//  RandomScheduler scheduler(engine);
//  Ptr<PMachine> mainMachine = Controller::createMainMachine(engine);
//  engine.run(scheduler, mainMachine);
//  return 0;
// }

#include <iostream>
#define DECL_TYPES List<Int, PTuple<Int, Int>, PList<PTuple<Int, Int>>, PMap<Int, Int>>
#define USE_VALUE_SUMMARY
#include "basic_cpp_runtime.h"

using namespace RUNTIME_NAMESPACE;

FUNCTION_DECL(Int, fib, (Int x), {
    IF(x <= 1)
    THEN({
        RETURN(1);
    }) 
    ELSE({
        RETURN(fib(x - 1) + fib(x - 2));
    })
    ENDIF()
})


FUNCTION_DECL(Int, sum, (Int x), {
    Int s = 0;
   	std::cout << x << std::endl;
    FOR(int i = 0, i < x, ++i, {
    	s = s + i;
    })
    ENDFOR()
    RETURN(s);
})

FUNCTION_DECL(void, entrance, (), {
   	ExecutionEngine engine = ExecutionEngine();
	Int x;
    IF(engine.randomBool("0"))
    THEN({
    	x = 10;
    })
    ELSE({
    	x = 11;
    })
    ENDIF();
    std::cout << fib(x) << std::endl;
    std::cout << fib(x) << std::endl;
    std::cout << sum(x) << std::endl;
})

int main(int argc, char const *argv[])
{
    PathConstraint::init();
    entrance();
    PathConstraint::quit();
    return 0;
}