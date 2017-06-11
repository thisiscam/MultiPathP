#include "basic_cpp_runtime.h"
#include <iostream>

using namespace basic_cpp_runtime;

DECL_TYPES(int, PAny, PList<int>, PList<PAny>, PTuple<int, int>, PList<PTuple<PAny, int>>);

int main(int argc, char const *argv[])
{
	/* code */
	// PTuple<int, int> t0(1, 1);
	// PAny a = any(t0);
	// PAny t1 = cast<PAny, PAny>(a);

	// std::cout << std::is_same<Parent<PAny>::value, ParentList<PAny>>::value  << std::endl;
	// std::cout << std::is_same<Parent<int>::value, ParentList<int, PAny>>::value << std::endl;
	// print_type<Parent<PMap<int, bool>>::value>();
	// print_type<Parent<PMap<PList<int>, PMap<bool, PAny>>>::value>();
	// print_type<Parent<PTuple<int, int>>::children>();
	PList<int> list;
	PList<PAny> anyList = cast<PList<PAny>>(list);
	list.add(0);
	anyList.add(cast<PAny>(PTuple<int, int>(13, 42)));
	std::cout << cast<PList<PTuple<PAny, int>>>(anyList).get(0).v1;
	print_type<Castables<PAny>::value>();
}