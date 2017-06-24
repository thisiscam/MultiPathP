#define USE_VALUE_SUMMARY
#include "header.h"

using namespace pingpong;

int main(int argc, char const *argv[])
{
    PathConstraint::init();
    ExecutionEngine engine = ExecutionEngine();
    RandomScheduler scheduler(engine);
    Ptr<PMachine> mainMachine = Controller::createMainMachine(engine);
    engine.run(scheduler, mainMachine);
    PathConstraint::quit();
 // PathConstraint::init();
 // Int x = 1;
 // std::cout << (x - 2) << std::endl;
 // PathConstraint::quit();
 return 0;
}

// #include <iostream>
// #define DECL_TYPES List<Int, PTuple<Int, Int>, PList<PTuple<Int, Int>>, PMap<Int, Int>, PMap<Int, PTuple<Int, Int>>>
// #define USE_VALUE_SUMMARY
// #include "basic_cpp_runtime.h"

// using namespace RUNTIME_NAMESPACE;

// // FUNCTION_DECL(Int, fib, (Int x), {
// //     IF(x <= 1)
// //     THEN({
// //         RETURN(1);
// //     }) 
// //     ELSE({
// //         RETURN(fib(x - 1) + fib(x - 2));
// //     })
// //     ENDIF()
// // })


// // FUNCTION_DECL(Int, sum, (Int x), {
// //     Int s = 0;
// //     FOR(int i = 0, i < x, ++i, {
// //      s = s + i;
// //     })
// //     ENDFOR()
// //     RETURN(s);
// // })

// FUNCTION_DECL(void, entrance, (), {
//     ExecutionEngine engine = ExecutionEngine();
//     PList<SendQueueItem> list;
//     IF(engine.randomBool("0"))
//     THEN({
//         list.add(SendQueueItem(NULL, 0, PAny()));
//     })
//     ELSE({
//         list.add(SendQueueItem(NULL, 3, PAny()));
//     })
//     ENDIF()
//     // IF(engine.randomBool("1")) 
//     // THEN({
//     //     list.add(23);
//     // })
//     // ELSE({
//     //     list.add(45);
//     // })
//     // ENDIF()
//     SendQueueItem x = list.get(0);
//     std::cout << x.e;
// })

// int main(int argc, char const *argv[])
// {
//     PathConstraint::init();
//  entrance();
//     PathConstraint::quit();
//     return 0;
// }