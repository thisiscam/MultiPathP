// #define DECL_TYPES List<int, PAny, PTuple<int, int>, PList<PTuple<int, int>> >

// #include "basic_cpp_runtime.h"
#include <iostream>
// #include "utils/type_helper.h"
// using namespace basic_cpp_runtime;

static int f(int i) {
	return i;
}

static int g() {
	auto&& a = f(3);
	return a;
}

int main(int argc, char const *argv[])
{
	auto&& a = f(1);
	auto&& b = f(2);
	std::cout << a + b << std::endl;
}