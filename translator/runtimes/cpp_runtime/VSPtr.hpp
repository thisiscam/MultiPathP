#ifndef VS_PTR_HPP
#define VS_PTR_HPP

#include <iostream>
#include <list>
#include <cassert>

namespace RUNTIME_NAMESPACE {

template<typename P>
class ValueSummary<P*> final {

public:

	ValueSummary():
		ValueSummary(NULL)
	{ }

	ValueSummary(P* v):values(std::list<Guard<P*>>(1, {PathConstraint::pc(), v}))
	{ }

	ValueSummary(const ValueSummary<P*>& other)
	{
		// TODO: better filter initialization
		for(const auto& gv : other.values) {
			Bdd pred = PathConstraint::pc() & gv.predicate;
			if (!pred.isZero()) {
				values.insert({pred, gv.value});
		 	}
		}
	}

	ValueSummary(const ValueSummary<P*>&& other):values(other.values) { }

	ValueSummary(ValueSummary<P*>&& other) = default;

	inline ValueSummary<P*>& operator= (const ValueSummary<P*>& rhs) {
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

	inline ValueSummary<bool> operator <(const ValueSummary<P*>& b) const {
		return binaryOp<bool>(*this, b, [](P* a, P* b) { return a < b; });
	}

	inline ValueSummary<bool> operator <=(const ValueSummary<P*>& b) const {
		return binaryOp<bool>(*this, b, [](P* a, P* b) { return a <= b; });
	}

	inline ValueSummary<bool> operator >(const ValueSummary<P*>& b) const {
		return binaryOp<bool>(*this, b, [](P* a, P* b) { return a > b; });
	}

	inline ValueSummary<bool> operator >=(const ValueSummary<P*>& b) const {
		return binaryOp<bool>(*this, b, [](P* a, P* b) { return a >= b; });
	}

	inline ValueSummary<bool> operator ==(const ValueSummary<P*>& b) const {
		return binaryOp<bool>(*this, b, [](P* a, P* b) { return a == b; });
	}

	inline ValueSummary<bool> operator !=(const ValueSummary<P*>& b) const {
		return binaryOp<bool>(*this, b, [](P* a, P* b) { return a != b; });
	}

	inline ValueSummary<P> operator *() const {
		return unaryOp<P>(*this, [](P* a) { return *a; });
	}

	friend ValueSummary<bool> operator >(const ValueSummary<P*>&, P*);
	friend ValueSummary<bool> operator >(int, const ValueSummary<P*>&);

	friend ValueSummary<bool> operator >=(const ValueSummary<P*>&, P*);
	friend ValueSummary<bool> operator >=(int, const ValueSummary<P*>&);

	friend ValueSummary<bool> operator <(const ValueSummary<P*>&, P*);
	friend ValueSummary<bool> operator <(int, const ValueSummary<P*>&);

	friend ValueSummary<bool> operator <=(const ValueSummary<P*>&, P*);
	friend ValueSummary<bool> operator <=(int, const ValueSummary<P*>&);

	friend ValueSummary<bool> operator ==(const ValueSummary<P*>&, P*);
	friend ValueSummary<bool> operator ==(int, const ValueSummary<P*>&);

	friend ValueSummary<bool> operator !=(const ValueSummary<P*>&, P*);
	friend ValueSummary<bool> operator !=(int, const ValueSummary<P*>&);

	template<typename ReturnType, typename A, typename B, typename BinOp>
	friend ValueSummary<ReturnType>
	binaryOp(const ValueSummary<A>& a, const ValueSummary<B>& b, BinOp&& binOp);

	template<typename ReturnType, typename A, typename UnaryOp>
	friend ValueSummary<ReturnType> 
	unaryOp(const ValueSummary<A>& a, UnaryOp&& uOp);

	friend std::ostream& operator<<(std::ostream&, const ValueSummary<P*>&);

private:

	ValueSummary(const std::unordered_map<int, Bdd>& values):values(values) { }

	std::list<Guard<P*>> values;
};

template<typename P>
inline ValueSummary<bool> operator>(const ValueSummary<P*>& a, P* b) {
	return unaryOp<bool>(a, [=](P* a) { return a > b; });
}

template<typename P>
inline ValueSummary<bool> operator>(P* b, const ValueSummary<P*>& a) {
	return unaryOp<bool>(a, [=](P* a) { return a > b; });
}

template<typename P>
inline ValueSummary<bool> operator>=(const ValueSummary<P*>& a, P* b) {
	return unaryOp<bool>(a, [=](P* a) { return a >= b; });
}

template<typename P>
inline ValueSummary<bool> operator>=(P* b, const ValueSummary<P*>& a) {
	return unaryOp<bool>(a, [=](P* a) { return a >= b; });
}

template<typename P>
inline ValueSummary<bool> operator<(const ValueSummary<P*>& a, P* b) {
	return unaryOp<bool>(a, [=](P* a) { return a < b; });
}

template<typename P>
inline ValueSummary<bool> operator<(P* b, const ValueSummary<P*>& a) {
	return unaryOp<bool>(a, [=](P* a) { return a < b; });
}

template<typename P>
inline ValueSummary<bool> operator<=(const ValueSummary<P*>& a, P* b) {
	return unaryOp<bool>(a, [=](P* a) { return a <= b; });
}

template<typename P>
inline ValueSummary<bool> operator<=(P* b, const ValueSummary<P*>& a) {
	return unaryOp<bool>(a, [=](P* a) { return a <= b; });
}

template<typename P>
inline ValueSummary<bool> operator==(const ValueSummary<P*>& a, P* b) {
	return unaryOp<bool>(a, [=](P* a) { return a == b; });
}

template<typename P>
inline ValueSummary<bool> operator==(P* b, const ValueSummary<P*>& a) {
	return unaryOp<bool>(a, [=](P* a) { return a == b; });
}

template<typename P>
inline ValueSummary<bool> operator!=(const ValueSummary<P*>& a, P* b) {
	return unaryOp<bool>(a, [=](P* a) { return a != b; });
}

template<typename P>
inline ValueSummary<bool> operator!=(P* b, const ValueSummary<P*>& a) {
	return unaryOp<bool>(a, [=](P* a) { return a != b; });
}

template<typename P>
std::ostream& operator<<(std::ostream& os, const ValueSummary<P*>& v)  
{  
	os << "VS[";
	int i = 0;
	for(const auto& gv : v.values) {
		if(i != 0) {
			os << ",";
		}
		os << gv.value;
		i++;
	}
	os << "]";
    return os;  
}

};

#endif