#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

#include <list>

#include "BddUtils.h"

namespace RUNTIME_NAMESPACE {

template<typename> class Allocator;

#ifdef USE_VALUE_SUMMARY

template<typename T>
class Allocator {

public:
	inline static Allocator* create(std::function<T*(int)> allocFunction) {
		Bdd oldPc = PathConstraint::pc();
		PathConstraint::pc() = Bdd::bddOne();
		Allocator* ret = new Allocator(allocFunction);
		PathConstraint::pc() = oldPc;
		return ret;
	}

	ValueSummary<T*> allocate() {
		ValueSummary<T*>&& ret = unaryOp<ValueSummary<T*>>(count, [&](int count) {
			if(count >= allocated.size()) {
				// Set PC to true, and set back
				// This way Allocated object's field will have initial PC of true
				Bdd oldPc = PathConstraint::pc();
				PathConstraint::pc() = Bdd::bddOne();
				allocated.push_back(allocFunction(count));
				PathConstraint::pc() = oldPc;
			}
			return allocated[count];
		});
		++count;
		return ret;
	}

private:
	std::function<T*(int)> allocFunction;

	vector<T*> allocated;

	Allocator(std::function<T*(int)> allocFunction):
		allocFunction(allocFunction),
		count(0)
	{ }

	Int count;
};


template<>
class Allocator<bool> {

public:
	inline static Allocator* create(const std::string& id) {
		Bdd oldPc = PathConstraint::pc();
		PathConstraint::pc() = Bdd::bddOne();
		Allocator* ret = new Allocator(id);
		PathConstraint::pc() = oldPc;
		return ret;
	}

	ValueSummary<bool> allocate() {
		ValueSummary<bool>&& ret = unaryOp<ValueSummary<bool>>(count, [&](int count) {
			if(count >= allocated.size()) {
				allocated.push_back(newBoolVar(id + "_" + std::to_string(count)));
			}
			ValueSummary<bool>::Builder builder;
			return builder.addValue(allocated[count], true).addValue(!allocated[count], false).build();
		});
		++count;
		return ret;
	}

private:

	const std::string id;

	vector<Bdd> allocated;

	Allocator(const std::string& id):
		id(id),
		count(0)
	{ }

	Int count;
};

#else

template<typename T>
class Allocator {

public:
	inline static Allocator* create(std::function<T*(int)> allocFunction) {
		return new Allocator(allocFunction);
	}

	T* allocate() {
		allocated.push_back(allocFunction(allocated.size()));
		return allocated[allocated.size() - 1];
	}

private:
	std::function<T*(int)> allocFunction;

	vector<T*> allocated;

	Allocator(std::function<T*(int)> allocFunction):
		allocFunction(allocFunction)
	{ }
};

template<>
class Allocator<bool> {

public:
	inline static Allocator* create(const std::string& id) {
		return new Allocator(id);
	}

	bool allocate() {
		allocated.push_back(rand() % 2);
		return allocated[allocated.size() - 1];
	}

private:

	const std::string id;

	vector<bool> allocated;

	Allocator(const std::string& id):
		id(id)
	{ }
};

#endif
};

#endif