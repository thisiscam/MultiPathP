#ifndef PTYPES_H
#define PTYPES_H

#include <type_traits>

#include "PAny.hpp"
#include "PMachine.hpp"
#include "PList.hpp"
#include "PMap.hpp"
#include "PTuple.hpp"

namespace basic_cpp_runtime {
template<typename FROM, typename TO> TO cast(const FROM& from);

template<typename FROM, typename TO>
struct CastFunctor {
	static TO impl(const FROM& from);
};

/* T -> T */
template<typename T>
struct CastFunctor<T, typename std::enable_if<std::is_same<T, int>::value | std::is_same<T, bool>::value | std::is_same<T, PMachine*>::value | std::is_same<T, PAny>::value, T>::type> {
	static T impl(const T& from) {
		return from;
	}
};

/* T -> PAny, given that T != PAny*/
template<typename T>
struct CastFunctor<T, typename std::enable_if<!std::is_same<T, PAny>::value, PAny>::type> {
	static PAny impl(const T& from) {
		return any(from);
	}
};

/* PAny -> int */
template<>
struct CastFunctor<PAny, int> {
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
struct CastFunctor<PAny, bool> {
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
struct CastFunctor<PAny, PMachine*> {
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
struct CastFunctor<PList<E_FROM>, PList<E_TO>> {
	static PList<E_TO> impl(const PList<E_FROM>& list) {
		PList<E_TO> ret;
		for(int i = 0; i < list.size(); i++) {
			ret.add(cast<E_FROM, E_TO>(list.get(i)));
		}
		return ret;
	}
};

template<typename E_TO>
struct CastFunctor<PAny, PList<E_TO>> {
	static PList<E_TO> impl(const PAny& anyList) {
		
	}
}; 

template<typename FROM, typename TO>
TO cast(const FROM& from) { 
	return CastFunctor<FROM, TO>::impl(from); 
}

};

#endif