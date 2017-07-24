#ifndef BDD_UTILS_HPP
#define BDD_UTILS_HPP

#include <sylvan_obj.hpp>
#include <list>
#include <tuple>

using namespace sylvan;

namespace RUNTIME_NAMESPACE {

inline 
void printDot(const Bdd& bdd, const std::string& fname) {
    FILE* f = fopen((fname + ".dot").c_str(), "w+");
    bdd.PrintDot(f);
    fclose(f);
}

static inline
std::list<std::tuple<const std::string, Bdd>>& getAllocatedBools() {
	static std::list<std::tuple<const std::string, Bdd>> allocatedBools;
	return allocatedBools;
}

inline 
Bdd newBoolVar(const std::string& name) {
	Bdd&& T = Bdd::bddVar(getAllocatedBools().size());
	getAllocatedBools().push_back(std::make_tuple(name, T));
	return T;
}

inline
void printBoolVars() {
	int i = 0;
	for(const auto& pair : getAllocatedBools()) {
		std::cout << i << ": " << std::get<0>(pair) << std::endl;
		i++;
	}
}

}

#endif