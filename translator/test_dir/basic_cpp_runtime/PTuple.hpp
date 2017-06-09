#ifndef PTUPLE_HPP
#define PTUPLE_HPP

#include "PTypePtr.h"

namespace basic_cpp_runtime {

template<typename ...Ts>
class PTuple;

template <typename T0, typename T1>
class PTuple<T0, T1> final : public PTypePtr {

public:
	PTuple(const T0& v0, const T1& v1):v0(v0),v1(v1) { }

	T0 v0;
	T1 v1;
};

template <typename T0, typename T1, typename T2>
class PTuple<T0, T1, T2> final : public PTypePtr {

public:
	PTuple(const T0& v0, const T1& v1, const T2& v2):v0(v0),v1(v1),v2(v2) { }

	T0 v0;
	T1 v1;
	T2 v2;
};


};

#endif