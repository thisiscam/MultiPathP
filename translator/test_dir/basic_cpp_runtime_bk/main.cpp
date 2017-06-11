#include "basic_cpp_runtime.h"
#include <iostream>

using namespace basic_cpp_runtime;

int main(int argc, char const *argv[])
{
	/* code */
	PTuplePtr<PBoxedInt, PBoxedInt> x = PTuple<PBoxedInt, PBoxedInt>::create(PBoxedInt::create(0), PBoxedInt::create(1));
	std::cout << x->cast<PTuple<IPType, IPType>>()->get0()->equals(PBoxedInt::create(0).get());
}