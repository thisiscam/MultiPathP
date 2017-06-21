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

int main(int argc, char const *argv[])
{
    PathConstraint::init();
    fib(1);
    PathConstraint::quit();
    ValueSummary<int*> x;
    // test<2>();
    // sum(10);
    return 0;
}