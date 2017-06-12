#ifndef PTYPES_H
#define PTYPES_H

#include <type_traits>
#include <map>

#include "PAny.hpp"
#include "PMachine.hpp"
#include "PList.hpp"
#include "PMap.hpp"
#include "PTuple.hpp"

#include "PTypeCastables.h"

#include "utils/type_helper.h"

namespace basic_cpp_runtime {
template<typename TO, typename FROM> TO cast(const FROM& from);
template<typename A, typename B> bool equals(const A& a, const B& b);

template<typename TO, typename FROM>
struct CastFunctor {
	static TO impl(const FROM& from);
};

/* T -> T */
template<typename T>
struct CastFunctor<typename std::enable_if<std::is_same<T, int>::value | std::is_same<T, bool>::value | std::is_same<T, PMachine*>::value | std::is_same<T, PAny>::value, T>::type, T> {
	static T impl(const T& from) {
		return from;
	}
};

/* T -> PAny, given that T != PAny */
template<typename T>
struct CastFunctor<typename std::enable_if<!std::is_same<T, PAny>::value, PAny>::type, T> {
	static PAny impl(const T& from) {
		return any(from);
	}
};

/* PAny -> int */
template<>
struct CastFunctor<int, PAny> {
	static int impl(const PAny& any) {
		if(any.type == &typeid(int)) {
			return any.i;
		} else {
			throw bad_cast();
		}
	}
};

/* PAny -> bool */
template<>
struct CastFunctor<bool, PAny> {
	static bool impl(const PAny& any) {
		if(any.type == &typeid(bool)) {
			return any.b;
		} else {
			throw bad_cast();
		}
	}
};

/* PAny -> PMachine* */
template<>
struct CastFunctor<PMachine*, PAny> {
	static PMachine* impl(const PAny& any) {
		if(any.type == &typeid(PMachine*)) {
			return any.m;
		} else {
			throw bad_cast();
		}
	}
};

/* PList<a> -> PList<b> */
template<typename E_FROM, typename E_TO>
struct CastFunctor<PList<E_TO>, PList<E_FROM>> {
	static PList<E_TO> impl(const PList<E_FROM>& list) {
		PList<E_TO> ret;
		for(int i = 0; i < list.size; i++) {
			ret.add(cast<E_TO, E_FROM>(list.get(i)));
		}
		return ret;
	}
};

/* PTuple<a, b> -> PTuple<a', b'> */
template<typename F1, typename F2, typename T1, typename T2>
struct CastFunctor<PTuple<T1, T2>, PTuple<F1, F2>> {
	static PTuple<T1, T2> impl(const PTuple<F1, F2>& tuple) {
		return PTuple<T1, T2>(cast<T1>(tuple.v0), cast<T2>(tuple.v1));
	}
};

template<typename TO, typename FROM>
TO anyCastHelper(const PAny& any) {
	return cast<TO, FROM>(*(FROM*)any.ptr.get());
}

/* PAny -> Contrainer<E...> */
template<template<typename...> class Container, typename ...Es>
struct CastFunctor<Container<Es...>, PAny> {
	typedef Container<Es...> (*AnyCastFunctionPtr)(const PAny&);
	using map_type = const std::map<const type_info*, AnyCastFunctionPtr>;
	template<typename> struct Inner;
	template<typename ...FromTypes>
	struct Inner<CastablesList<FromTypes...>> {
		/* 
			This inner struct declares a static map<type_info, (any) -> Contrainer<E>>,
			Essentially, they form a virtual double dispatch table
		*/
		static inline map_type& jump_table() {
			static map_type _m = { {&typeid(FromTypes), anyCastHelper<Container<Es...>, FromTypes>} ...};
			return _m;
		}
	};
	static Container<Es...> impl(const PAny& any) {
		map_type table = Inner<typename Castables<Container<Es...>>::value>::jump_table();
		return table.at(any.type)(any);
	}
};

template<typename TO, typename FROM>
TO cast(const FROM& from) { 
	return CastFunctor<TO, FROM>::impl(from); 
}

template<typename A, typename B>
bool equals(const A& a, std::enable_if<!std::is_same<A, B>::value, const B&>::type from) { 
	return equals(cast<B>(a), b);
}

template<>
bool equals<int, int>(const int& a, const int& b) {
	return a == b;
}

};

#endif