#define DECL_TYPES List<int, PAny, PTuple<int, int>, PList<PTuple<int, int>> >

#include "basic_cpp_runtime.h"
#include "utils/type_helper.h"
using namespace basic_cpp_runtime;

struct A {
	inline void f() const {
		printf("const\n");
	}

	inline void f() {
		printf("yy\n");
	}
};

int main(int argc, char const *argv[])
{
    PList<PTuple<int, int>> list;
    list.add(PTuple<int, int>(1, 1));
    list.getl(0).v0l() = 2;
    std::cout << list.get(0).v0() << std::endl;
    A().f();
}