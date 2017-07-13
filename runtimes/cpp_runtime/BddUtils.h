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

inline 
Bdd newBoolVar(const std::string& name) {
	static std::list<std::tuple<const std::string, Bdd>> allocatedBools;
	Bdd&& T = Bdd::bddVar(allocatedBools.size());
	allocatedBools.push_back(std::make_tuple(name, T));
	return T;
}

}

#endif