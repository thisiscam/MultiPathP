#ifndef PTUPLE_HPP
#define PTUPLE_HPP

#include "PTypePtr.h"

namespace basic_cpp_runtime {

template<typename ...Ts>
class PTuple;

template <typename T0, typename T1>
class PTuple<T0, T1> final : public PTypePtr {

public:
	PTuple() = default;
	PTuple(const T0& v0, const T1& v1):v0(v0),v1(v1) { }

	template<typename T0p, typename T1p>
	explicit operator PTuple<T0p, T1p>() {
		return PTuple<T0p, T1p>(static_cast<T0p>(v0), static_cast<T1p>(v1));
	}

	bool operator == (const PTuple<T0, T1>& other) {
		return v0 == other.v0 && v1 == other.v1;
	}

	bool operator != (const PTuple<T0, T1>& other) {
		return !(*this == other);
	}

	T0 v0;
	T1 v1;
};

template <typename T0, typename T1, typename T2>
class PTuple<T0, T1, T2> final : public PTypePtr {

public:
	PTuple(const T0& v0, const T1& v1, const T2& v2):v0(v0),v1(v1),v2(v2) { }

	template<typename T0p, typename T1p, typename T2p>
	explicit operator PTuple<T0p, T1p, T2p>() {
		return PTuple<T0p, T1p>(static_cast<T0p>(v0), static_cast<T1p>(v1), static_cast<T2p>(v2));
	}

	bool operator == (const PTuple<T0, T1, T2>& other) {
		return v0 == other.v0 && v1 == other.v1 && v2 == other.v2;
	}

	bool operator != (const PTuple<T0, T1, T2>& other) {
		return !(*this == other);
	}

	T0 v0;
	T1 v1;
	T2 v2;
};


};

#endif