#ifndef PTYPE_PARENT_H
#define PTYPE_PARENT_H

#include <type_traits>
#include "utils/crossproduct.hpp"

namespace basic_cpp_runtime {

template<typename ...> struct CastablesList {
	
};

template<typename T> struct Castables;

template <typename, typename> struct Merge;

template <template <typename...> class P, typename... Ts, typename... Us>
struct Merge<P<Ts...>, P<Us...>> {
    using value = P<Ts..., Us...>;
};

template<typename, typename> struct SetAdd;
template<template <typename...> class P, typename U, typename T, typename ...Tails>
struct SetAdd<U, P<T, Tails...>> {
	using value = typename std::conditional<std::is_same<U, T>::value, P<T, Tails...>, typename Merge<P<T>, typename SetAdd<U, P<Tails...>>::value>::value>::type;
};
template<template <typename...> class P, typename U>
struct SetAdd<U, P<>> {
	using value = P<U>;
};

template <typename> struct MkSet;
template <template <typename...> class P, typename T, typename ...Tails>
struct MkSet<P<T, Tails...>> {
	using value = typename SetAdd<T, typename MkSet<P<Tails...>>::value>::value;
};
template <template <typename...> class P>
struct MkSet<P<>> {
	using value = P<>;
};

#define DECL_TYPES(args...) \
	template<> \
	struct Castables<PAny> { \
		using value = MkSet<CastablesList<args>>::value; \
	};

template<>
struct Castables<int> {
	using value = CastablesList<int, PAny>;
};

template<>
struct Castables<bool> {
	using value = CastablesList<bool, PAny>;
};

template<>
struct Castables<PMachine*> {
	using value = CastablesList<PMachine*, PAny>;
};

template<typename E>
struct Castables<PList<E>> {

	template<typename> struct MapPList;

	template<typename ...Childs>
	struct MapPList<CastablesList<Childs...>> {
		using value = CastablesList<PList<Childs>...>;
	};

	using value = typename MkSet<typename Merge<typename MapPList<typename Castables<E>::value>::value, CastablesList<PAny>>::value>::value;
};

template<typename K, typename V>
struct Castables<PMap<K, V>> {

	template<typename> struct MapPMap;

	template<typename ...Ks, typename ...Vs>
	struct MapPMap<CastablesList<CastablesList<Ks, Vs>...>> {
		using value = CastablesList<PMap<Ks, Vs>...>;
	};

	using children = typename MapPMap<typename product<CastablesList, typename Castables<K>::value, typename Castables<V>::value>::type>::value;
	using value = typename MkSet<typename Merge<children, CastablesList<PAny>>::value>::value;
};

template<typename ...Ts>
struct Castables<PTuple<Ts...>> {
	template<typename> struct MapPTuple;
	template<typename ...Tuples>
	struct MapPTuple<CastablesList<Tuples...>> {
		template<typename> struct Inner;
		template<typename ...Es>
		struct Inner<CastablesList<Es...>> {
			using value = PTuple<Es...>;
		};
		using value = CastablesList<typename Inner<Tuples>::value...>;
	};
	using children = typename MapPTuple<typename product<CastablesList, typename Castables<Ts>::value...>::type>::value;
	using value = typename MkSet<typename Merge<children, CastablesList<PAny>>::value>::value;
};

};

#endif