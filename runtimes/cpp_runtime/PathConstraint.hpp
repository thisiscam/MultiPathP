#ifndef PATH_CONSTRAINT_HPP
#define PATH_CONSTRAINT_HPP

#include <sylvan_obj.hpp>
#include <iostream>
#include <stdio.h>

#include "BddUtils.h"

using namespace sylvan;

namespace RUNTIME_NAMESPACE {

template<typename>
class ValueSummary;

class PathConstraint final {

public:

    static inline void init(
                size_t min_tablesize_lg2 = 24, 
                size_t max_tablesize_lg2 = 27,
                size_t min_cachesize_lg2 = 24, 
                size_t max_cachesize_lg2 = 27,
                size_t n_workers = 1) 
    {
        lace_init(n_workers, 1000000UL);
        lace_startup(0, NULL, NULL);

        size_t min_tablesize = 1UL << min_tablesize_lg2;
        size_t max_tablesize = 1UL << max_tablesize_lg2;
        size_t min_cachesize = 1UL << min_cachesize_lg2;
        size_t max_cachesize = 1UL << max_cachesize_lg2;

        /* TODO: bump sylvan version when hwloc issue is resolved */
        sylvan_set_sizes(min_tablesize, max_tablesize, min_cachesize, max_cachesize);

        sylvan_init_package();
        sylvan_init_mtbdd();

        pc() = Bdd::bddOne();
    }

    static inline void quit() {
        sylvan_stats_report(stdout);
        sylvan_quit();
        lace_exit();
    }

    static inline Bdd& pc() {
        static Bdd _pc;
        return _pc;
    }

    static inline bool isNotZero() {
        return !pc().isZero();
    }

    static inline bool isZero() {
        return pc().isZero();
    }

    static inline void printDot(const std::string& fname) {
        vs_cpp_runtime::printDot(pc(), fname);
    }
};

};

#endif