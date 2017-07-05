#ifndef SET_HPP
#define SET_HPP

#include <unordered_map>
#include <set>

namespace RUNTIME_NAMESPACE {

#ifndef USE_VALUE_SUMMARY
template<typename T>
class Set {

public:
	inline void add(const T& value) {
		values.insert(value);
	}

	template<typename Function>
	inline void foreach(Function&& iter) {
		for(const T& v: values) {
			iter(v);
		}
	}

	inline int size() {
		return values.size();
	}

private:
	std::set<T> values;

};

#else

template<typename T>
class Set {

public:
	inline void add(const ValueSummary<T>& value) {
		unaryOp<void>(value, [&](const T& v) {
			values[v] |= PathConstraint::pc();
		});
	}

	template<typename Function>
	inline void foreach(Function&& iter) {
    	for(const auto& gv : values) {
    		Bdd oldPc = PathConstraint::pc();
    		PathConstraint::pc() &= gv.second;
    		if(PathConstraint::isNotZero()) {
    			iter(gv.first);
    		}
    		PathConstraint::pc() = oldPc;
    	}
	}


	inline int size() {
		//TODO might want to return VS<int>
		return values.size();
	}

private:
	std::unordered_map<T, Bdd> values;

};

#endif
};

#endif