#include "basic_cpp_runtime.h"
#include <iostream>

using namespace basic_cpp_runtime;

#include <iostream>
#include <type_traits>
#include <cxxabi.h>
#include "utils/crossproduct.hpp"

template<typename ...> struct ParentList {
	
};

template<typename T> struct Parent;

template <typename, typename> struct Merge;

template <template <typename...> class P, typename... Ts, typename... Us>
struct Merge<P<Ts...>, P<Us...>> {
    using value = P<Ts..., Us...>;
};

template<>
struct Parent<PAny> {
	using value = ParentList<PAny>;
};

template<>
struct Parent<int> {
	using value = ParentList<int, PAny>;
};

template<>
struct Parent<bool> {
	using value = ParentList<bool, PAny>;
};

template<>
struct Parent<PMachine*> {
	using value = ParentList<PMachine*, PAny>;
};

template<typename E>
struct Parent<PList<E>> {

	template<typename> struct MapPList;

	template<typename ...Childs>
	struct MapPList<ParentList<Childs...>> {
		using value = ParentList<PList<Childs>...>;
	};

	using value = typename Merge<typename MapPList<typename Parent<E>::value>::value, ParentList<PAny>>::value;
};

template<typename K, typename V>
struct Parent<PMap<K, V>> {

	template<typename> struct MapPMap;

	template<typename ...Ks, typename ...Vs>
	struct MapPMap<ParentList<ParentList<Ks, Vs>...>> {
		using value = ParentList<PMap<Ks, Vs>...>;
	};

	using children = typename MapPMap<typename product<ParentList, typename Parent<K>::value, typename Parent<V>::value>::type>::value;
	using value = typename Merge<children, ParentList<PAny>>::value;
};

template<typename T>
void print_type() {
	int s;
  	std::cout << abi::__cxa_demangle(typeid(T).name(), 0, 0, &s) << std::endl;
}

int main(int argc, char const *argv[])
{
	/* code */
	// PTuple<int, int> t0(1, 1);
	// PAny a = any(t0);
	// PAny t1 = cast<PAny, PAny>(a);

	// std::cout << std::is_same<Parent<PAny>::value, ParentList<PAny>>::value  << std::endl;
	// std::cout << std::is_same<Parent<int>::value, ParentList<int, PAny>>::value << std::endl;
	// print_type<Parent<PMap<int, bool>>::value>();
	print_type<Parent<PMap<PList<int>, PMap<bool, PAny>>>::value>();
}