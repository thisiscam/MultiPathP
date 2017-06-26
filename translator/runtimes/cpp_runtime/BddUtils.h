#ifndef BDD_UTILS_HPP
#define BDD_UTILS_HPP

#include <sylvan_obj.hpp>

using namespace sylvan;

namespace RUNTIME_NAMESPACE {

inline void printDot(const Bdd& bdd, const std::string& fname) {
    FILE* f = fopen((fname + ".dot").c_str(), "w+");
    bdd.PrintDot(f);
    fclose(f);
}

}

#endif