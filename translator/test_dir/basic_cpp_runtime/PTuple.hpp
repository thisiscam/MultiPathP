#ifndef PTUPLE_HPP
#define PTUPLE_HPP

#include "IPType.hpp"

using namespace std;

namespace basic_cpp_runtime {

template<typename T0, typename T1> class PTuple;
template<typename T0, typename T1> using PTuplePtr = unique_ptr<PTuple<T0, T1>>;

template <typename T0, typename T1>
class PTuple : public IPType {
	template<typename U0, typename U1> friend class PTuple;

	using T0Ptr = std::unique_ptr<T0>;
	using T1Ptr = std::unique_ptr<T1>;

private:
	struct PTupleKind {
		const char* tag = "PTuple2Kind";
		PTupleKind():t0(T0::kind()),t1(T1::kind()) {}
		const IPType::Kind* t0;
		const IPType::Kind* t1;
	};
	const static PTupleKind _kind;

public:
	static constexpr const PTupleKind* kind() {
		return &_kind;
	}

	PTuple(T0Ptr v0, T1Ptr v1):v0(std::move(v0)),v1(std::move(v1)) { }

	static PTuplePtr<T0, T1> create(T0Ptr v0, T1Ptr v1) {
		return PTuplePtr<T0, T1>(new PTuple<T0, T1>(std::move(v0), std::move(v1)));
	}

	T0Ptr get0() {
		return v0->copy();
	}

	T0Ptr get1() {
		return v1->copy();
	}

	void set0(T0Ptr value) {
		return v0 = std::move(value);
	}

	void set1(T1Ptr value) {
		return v1 = std::move(value);
	}

	IPTypePtr dynamicCopy() override {
		return std::move(copy());
	}

	PTuplePtr<T0, T1> copy() {
		return create(v0->copy(), v1->copy());
	}

	bool equals(const IPType* other) const override {
		if(!dynamic_cast<const PTuple<T0, T1>*>(other) && !dynamic_cast<const PTuple<T0, T1>*>(other)) {
			return false;
		}
		const PTuple<IPType, IPType>* o = static_cast<const PTuple<IPType, IPType>*>(other);
		return v0->equals(o->v0.get()) && v1->equals(o->v1.get());
	}

	void checkType(const IPType::Kind* targetKind) override {
		if(targetKind != (IPType::Kind*)kind() && targetKind != IPType::kind()) { // tuple<T0, T1> or any
			if(targetKind->tag != _kind.tag) {
				throw new bad_cast();
			}
			PTupleKind* targetTupleKind = (PTupleKind*)targetKind;
			if(targetTupleKind->t0 != IPType::kind()) { // tuple<any, *> 
				v0->checkType(targetTupleKind->t0);
			}
			if(targetTupleKind->t1 != IPType::kind()) { // tuple<*, any>
				v1->checkType(targetTupleKind->t1);
			}
		}
	}

private:
	T0Ptr v0;
	T1Ptr v1;
};

template<typename T0, typename T1>
const typename PTuple<T0, T1>::PTupleKind PTuple<T0, T1>::_kind = PTuple<T0, T1>::PTupleKind();

};

#endif