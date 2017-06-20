#include <iostream>
#define DECL_TYPES List<int, PAny, PTuple<int, int>, PList<PTuple<int, int>> >
#include "basic_cpp_runtime.h"

using namespace RUNTIME_NAMESPACE;

int fib(int x) {
	BEGIN_BODY(int)
		IF(x <= 1)
		THEN({
			RETURN(1);
		}) 
		ELSE({
			RETURN(fib(x - 1) + fib(x - 2));
		})
		ENDIF()
	END_BODY()
}

int main(int argc, char const *argv[])
{
	PathConstraint::init();
	fib(10);
	PathConstraint::quit();
	return 0;
}