// #include "header.h"

// using namespace pingpong;

// int main(int argc, char const *argv[])
// {
// 	ExecutionEngine engine = ExecutionEngine();
// 	RandomScheduler scheduler(engine);
// 	Ptr<PMachine> mainMachine = Controller::createMainMachine(engine);
// 	engine.run(scheduler, mainMachine);
// 	return 0;
// }

#include <iostream>
#define DECL_TYPES List<int, PAny, PTuple<int, int>, PList<PTuple<int, int>>>
#include "basic_cpp_runtime.h"

using namespace RUNTIME_NAMESPACE;

FUNCTION_DECL(ValueSummary<int>, fib, (ValueSummary<int> x), {
	IF(x <= 1)
	THEN({
		RETURN(1);
	}) 
	ELSE({
		RETURN(fib(x - 1) + fib(x - 2));
	})
	ENDIF()
})

// FUNCTION_DECL(ValueSummary<int>, sum, (ValueSummary<int> x), {
// 	ValueSummary<int> sum = 0;
// 	WHILE((x > 1), {
// 		sum = sum + x;
// 		std::cout << x << std::endl;
// 		x = x - 1;
// 	})
// 	RETURN(sum);
// })

// template<int x>
// FUNCTION_DECL(void, test, (), {
// 	printf("yeah %d\n", x);
// })

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