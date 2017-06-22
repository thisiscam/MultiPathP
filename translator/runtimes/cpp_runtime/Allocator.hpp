#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

namespace RUNTIME_NAMESPACE {

template<typename T>
class Allocator {

public:
	Allocator(std::function<T(void)> allocator):
		values(allocator),
		count(0)
	{ }

	T allocate() {
#ifdef USE_VALUE_SUMMARY
		Bdd oldPc = PathConstraint::pc();
		PathConstraint::pc() = Bdd::bddOne();
		T ret = values.get(count);
		PathConstraint::pc() = oldPc;
#else
		T ret = values.get(count);
#endif
		++count;
		return ret;
	}

private:

	DefaultArray<T> values;

	Int count;
};

};

#endif