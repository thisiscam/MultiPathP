// #define USE_VALUE_SUMMARY
#include "header.h"

using namespace pingpong;

int main(int argc, char const *argv[])
{
    // PathConstraint::init();
    // for(int i = 0; i < 1000000; i++) {
		ExecutionEngine engine = ExecutionEngine();
	    RandomScheduler scheduler(engine);
	    Ptr<PMachine> mainMachine = Controller::createMainMachine(engine);
	    engine.run(scheduler, mainMachine);
    // }

    // PathConstraint::quit();
 // PathConstraint::init();
 // Int x = 1;
 // std::cout << (x - 2) << std::endl;
 // PathConstraint::quit();
 return 0;
}

// #include <iostream>
// #define DECL_TYPES List<Int, PTuple<Int, Int>, PList<PTuple<Int, Int>>, PMap<Int, Int>, PMap<Int, PTuple<Int, Int>>>
// #define USE_VALUE_SUMMARY
// #include "PCppRuntime.h"

// using namespace RUNTIME_NAMESPACE;

// FUNCTION_DECL(Int, fib, (Int x)) {
//     IF(x <= 1)
//     THEN() {
//         RETURN(1);
//     }
//     ELSE() {
//         RETURN(fib(x - 1) + fib(x - 2));
//     }
//     ENDIF()
// }
// END_FUNCTION()

// FUNCTION_DECL(Int, sum, (Int x)) {
//     Int s = 0;
//     FOR(int i = 0, i < x, ++i) {
//     	s = s + i;
//     }
//     ENDFOR()
//     RETURN(s);
// }
// END_FUNCTION()

// VOID_FUNCTION_DECL(entrance, ()) {
//     ExecutionEngine engine = ExecutionEngine();
//     PList<SendQueueItem> list;
//     IF(engine.randomBool("0"))
//     THEN() {
//         list.add(SendQueueItem(NULL, 10, PAny()));
//     }
//     ELSE() {
//         list.add(SendQueueItem(NULL, 9, PAny()));
//     }
//     ENDIF()   	
//     SendQueueItem x = list.get(0);
//     std::cout << x.e;
//     std::cout << fib(list.get(0).e) << std::endl;
//     std::cout << sum(list.get(0).e) << std::endl;
// }
// END_VOID_FUNCTION()

// int main(int argc, char const *argv[])
// {
//     PathConstraint::init();
//  	entrance();
//     PathConstraint::quit();
//     return 0;
// }