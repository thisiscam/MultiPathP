#ifndef PANY_HPP
#define PANY_HPP

#include <memory>
#include <stdexcept>
#include <typeinfo>
#include <map>
#include <iostream>

#include "PTypePtr.h"
#include "utils/crossproduct.hpp"

using namespace std;

namespace basic_cpp_runtime {

template<typename T>
struct IsSimpleType
{
	static constexpr bool value = std::is_same<T, int>::value || std::is_same<T, bool>::value || std::is_same<T, PMachine*>::value;
};

template<typename, typename> struct IsCastable {
	static constexpr bool value = false;
};

template<typename T>
struct IsCastable<T, typename std::enable_if<IsSimpleType<T>::value, T>::type> {
	static constexpr bool value = true;
};

template<typename T>
struct IsCastable<PAny, T> {
	static constexpr bool value = true;
};

template<template <typename ...Ts> class Container>
struct IsCastable<Container<>, Container<>> {
	static constexpr bool value = true;
};

template<template <typename ...Ts> class Container, typename A, typename B, typename ...As, typename ...Bs>
struct IsCastable<Container<A, As...>, Container<B, Bs...>> {
	static constexpr bool value = IsCastable<A, B>::value && IsCastable<List<As...>, List<Bs...>>::value;
};


class PAny final {
private:
	template<typename FROM, typename TO>
	static TO CastHelper(typename std::enable_if<IsCastable<FROM, TO>::value, const PAny&>::type from) {
		return static_cast<TO>(*(FROM*)from.ptr.get());
	}

	template<typename FROM, typename TO>
	static TO CastHelper(typename std::enable_if<!IsCastable<FROM, TO>::value, const PAny&>::type from) {
		throw bad_cast();
	}

	template<typename, typename, typename=void>
	struct EqHelperFunctor;

public:
	PAny(const type_info& type, int i, bool b, PMachine* m, PTypePtr* ptr):
		type(&type),
		i(i),b(b),m(m),ptr(ptr) 
	{ }
	
	PAny():PAny(typeid(int), 0, false, NULL, NULL) { };

	PAny(const PAny& other) = default;

	PAny(PAny&& other) = default;

	PAny& operator=(const PAny& other) = default;

	PAny& operator=(PAny&& other) = default;

	template<typename T>
	PAny(const T& v):PAny(typeid(T), 0, false, NULL, new T(v)) { }

	PAny(const int& v):PAny(typeid(int), v, false, NULL, NULL) { }

	PAny(const bool& v):PAny(typeid(bool), 0, v, NULL, NULL) { }

	PAny(PMachine* const & v):PAny(typeid(bool), 0, false, v, NULL) { }

	inline explicit operator int() {
		if(type == &typeid(int)) {
			return i;
		} else {
			throw bad_cast();
		}
	}

	inline explicit operator bool() {
		if(type == &typeid(bool)) {
			return b;
		} else {
			throw bad_cast();
		}
	}

	inline explicit operator PMachine*() {
		if(type == &typeid(PMachine*)) {
			return m;
		} else {
			throw bad_cast();
		}
	}

	template<typename TO>
	using CastFunctionPointer = TO (*)(const PAny&);
	template<typename TO>
	using CastTableType = std::map<const type_info*, CastFunctionPointer<TO>>;
	template<typename, typename> struct CastJumpTable;
	template<typename ...FROMs, typename TO>
	struct CastJumpTable<List<FROMs...>, TO> {
		static CastTableType<TO>& table() {
			static CastTableType<TO> _table = { {&typeid(FROMs), &PAny::CastHelper<FROMs, TO>} ... };
			return _table;
		}
	};

	template<template<typename ...> class Container, typename ...Ts>
	explicit operator Container<Ts...>() {
		return CastJumpTable<DECL_TYPES, Container<Ts...>>::table().at(type)(*this);
	}


	using EqFunctionPointer = bool (*)(const PAny&, const PAny&);
	using EqTableKeyType = std::tuple<const type_info*, const type_info*>;
	using EqTableType = std::map<EqTableKeyType, EqFunctionPointer>;
	template<typename> struct EqJumpTable;
	template<typename ...Pair>
	struct EqJumpTable<List<Pair...>> {
		template<typename> struct Extract;
		template<typename A, typename B>
		struct Extract<List<A, B>> {
			using a = A;
			using b = B;
		};
		static EqTableType& table() {
			static EqTableType _table = 
				{ 	
					{ 
						{
							&typeid(typename Extract<Pair>::a), 
							&typeid(typename Extract<Pair>::b) 
						}, 
						&PAny::EqHelperFunctor<typename Extract<Pair>::a, typename Extract<Pair>::b>::impl
					} 
			... };
			return _table;
		}
	};

	bool operator == (const PAny& other) const {
		return EqJumpTable<product<List, DECL_TYPES, DECL_TYPES>::type>::table().at(make_tuple(type, other.type))(*this, other);
		return false;
	}

	bool operator != (const PAny& other) const {
		return !(*this == other);
	}

	const type_info* type;
	int i;
	bool b;
	PMachine* m;
	shared_ptr<PTypePtr> ptr;
};

template<> 
struct PAny::EqHelperFunctor<int, int> {
	static bool impl(const PAny& a, const PAny& b) {
		return a.i == b.i;
	}
};

template<> 
struct PAny::EqHelperFunctor<bool, bool> {
	static bool impl(const PAny& a, const PAny& b) {
		return a.b == b.b;
	}
};

template<> 
struct PAny::EqHelperFunctor<PMachine*, PMachine*> {
	static bool impl(const PAny& a, const PAny& b) {
	return a.m == b.m;
	}
};

template<typename A, typename B> 
struct PAny::EqHelperFunctor<A, B, typename std::enable_if<!IsCastable<A, B>::value>::type> {
	static bool impl(const PAny& a, const PAny& b) {
		return false;
	}
};

template<typename A, typename B>
struct PAny::EqHelperFunctor<A, B, typename std::enable_if<IsCastable<A, B>::value && !IsSimpleType<A>::value>::type> {
	static bool impl(const PAny& a, const PAny& b) {
		return *(A*)a.ptr.get() == static_cast<A>(*(B*)b.ptr.get());
	}
};

};

#endif