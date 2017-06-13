#define DECL_TYPES List<int, PAny, PList<int>, PList<PAny>, bool>

#include "basic_cpp_runtime.h"
#include <iostream>
#include "utils/type_helper.h"
using namespace basic_cpp_runtime;

int main(int argc, char const *argv[])
{
	/* code */
	// PTuple<int, int> t0(1, 1);
	// PAny a = any(t0);
	// PAny t1 = static_cast<PAny, PAny>(a);

	// std::cout << std::is_same<Parent<PAny>::value, ParentList<PAny>>::value  << std::endl;
	// std::cout << std::is_same<Parent<int>::value, ParentList<int, PAny>>::value << std::endl;
	// print_type<Parent<PMap<int, bool>>::value>();
	// print_type<Parent<PMap<PList<int>, PMap<bool, PAny>>>::value>();
	// print_type<Parent<PTuple<int, int>>::children>();
	// 
	// PTuple<int, PAny> a = static_cast<PTuple<int, PAny>>(PTuple<int, int>(0, 1));
	// std::cout << static_cast<int>(a.v1);
	// static_cast<PTuple<int, int>>(static_cast<PAny>(1));
	PList<int> x;
	x.add(42);
	x.add(41);
	PList<int> y = x;
	PList<PAny> a = static_cast<PList<PAny>>(x);
	PList<PAny> b = static_cast<PList<PAny>>(y);
	a.add(static_cast<PAny>(true));
	b.add(static_cast<PAny>(true));
	std::cout << (a == b) << std::endl;
}