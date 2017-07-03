#ifndef BDD_UTILS_HPP
#define BDD_UTILS_HPP

#include <sylvan_obj.hpp>
#include <list>

using namespace sylvan;

namespace RUNTIME_NAMESPACE {

inline 
void printDot(const Bdd& bdd, const std::string& fname) {
    FILE* f = fopen((fname + ".dot").c_str(), "w+");
    bdd.PrintDot(f);
    fclose(f);
}

 
inline 
std::list<std::tuple<const std::string, Bdd>>& bddAllocationList() {
	static std::list<std::tuple<const std::string, Bdd>> allocatedBools;
	return allocatedBools;
}

inline 
Bdd newBoolVar(const std::string& name) {
	auto& allocatedBools = bddAllocationList();
	Bdd&& T = Bdd::bddVar(allocatedBools.size());
	allocatedBools.push_back(std::make_tuple(name, T));
	return T;
}

}

#endif