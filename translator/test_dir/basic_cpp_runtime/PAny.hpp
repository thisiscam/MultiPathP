#ifndef PANY_HPP
#define PANY_HPP

#include <memory>
#include <stdexcept>
#include <typeinfo>
#include <unordered_map>

#include "PTypePtr.h"

using namespace std;

namespace basic_cpp_runtime {

class PMachine;

struct PAny final {
	PAny(const type_info& type, int i, bool b, PMachine* m, PTypePtr* ptr):
		type(&type),
		i(i),b(b),m(m),ptr(ptr) 
	{ }
	
	PAny():PAny(typeid(int), 0, false, NULL, NULL) { };

	const type_info* type;
	int i;
	bool b;
	PMachine* m;
	shared_ptr<PTypePtr> ptr;
};

template<typename T> inline 
PAny any(const T& v) {
	return PAny(typeid(T), 0, false, NULL, new T(v));
}

template<> inline
PAny any(const int& v) {
	return PAny(typeid(int), v, false, NULL, NULL);
}

template<> inline
PAny any(const bool& v) {
	return PAny(typeid(bool), 0, v, NULL, NULL);
}

template<> inline
PAny any(PMachine* const & v) {
	return PAny(typeid(bool), 0, false, v, NULL);
}

};

#endif