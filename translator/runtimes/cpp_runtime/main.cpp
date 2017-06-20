// struct A {
// 	inline void f() const {
// 		printf("const\n");
// 	}

// 	inline void f() {
// 		printf("yy\n");
// 	}
// };

// int main(int argc, char const *argv[])
// {
//     PList<PTuple<int, int>> list;
//     list.add(PTuple<int, int>(1, 1));
//     list.getl(0).v0l() = 2;
//     std::cout << list.get(0).v0() << std::endl;
//     A().f();
// }

#include <iostream>
#define DECL_TYPES List<Int, PAny, PTuple<Int, Int>, PList<PTuple<Int, Int>> >
#include "basic_cpp_runtime.h"

using namespace RUNTIME_NAMESPACE;

Int fib(Int x) {
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
	fib(3);
	PathConstraint::quit();
	return 0;
}