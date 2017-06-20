#ifndef VS_HPP
#define VS_HPP

#include <list>

namespace RUNTIME_NAMESPACE {

template<typename T>
class ValueSummary final {

	ValueSummary():
		ValueSummary(T())
	{ }

	ValueSummary(const T& v):values(std::list<Guard<T>>(1, {PathConstraint::pc(), v}))
	{ }

	ValueSummary(const ValueSummary<T>& other)
	{
		// TODO: better filter initialization
		for(const auto& gv : other.values) {
			Bdd pred = PathConstraint::pc() & gv.predicate;
			if (!pred.isZero()) {
				values.insert({pred, gv.value});
		 	}
		}
	}

	ValueSummary(const ValueSummary<T>&& other):values(other.values) { }

	ValueSummary(ValueSummary<T>&& other) = default;

	inline ValueSummary<T>& operator= (const ValueSummary<T>& rhs) {
		for(const auto& gvRhs : rhs.values) {
			Bdd pred = PathConstraint::pc() & gvRhs.predicate;
			if(!pred.isZero()) {
				for(const auto& gvLhs : values) {
					gvLhs.predicate &= !pred;
				}
				values.push_back({pred, gvRhs.value});
			}
		}
		return *this;
	}


private:

	std::list<Guard<T>> values;
};

};
#endif