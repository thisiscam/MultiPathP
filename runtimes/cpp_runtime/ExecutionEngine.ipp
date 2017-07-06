#ifndef EXECUTION_ENGINE_IPP
#define EXECUTION_ENGINE_IPP

#include <map>
#include <string>
#include <vector>
#include <tuple>
#include <math.h>

#include "Scheduler.hpp"

namespace RUNTIME_NAMESPACE {

inline void 
ExecutionEngine::run(Scheduler& scheduler, Ptr<PMachine> machine) {
    scheduler.startMachine(machine);
    for(int i = 0; i < maxIteration; ++i) {
    	std::cout << "======== BEGIN Step " << std::to_string(i) << "=======" << std::endl;
        IF_ONLY(scheduler.step()) {
            continue;
        } else {
            break;
        }
        std::cout << "======== END Step " << std::to_string(i) << "=======" << std::endl;
    }
}

inline Bool
ExecutionEngine::randomBool(const std::string& id) {
	static std::map<const std::string, Allocator<bool>*> allocators;
	if(allocators.count(id) == 0) {
		allocators.insert({id, Allocator<bool>::create(id)});
	}
	return allocators.at(id)->allocate();
}

static inline
std::vector<Bdd> buildTreePaths(const Bdd* vars, const int maxDecisions)
{
    if (maxDecisions == 1) {
        return std::vector<Bdd>(1, Bdd::bddOne());
    } else {
        int left = maxDecisions / 2 + maxDecisions % 2;
        int right = maxDecisions - left;
        std::vector<Bdd> leftPaths = std::move(buildTreePaths(vars, left));
        auto&& rightPaths = buildTreePaths(vars, right);
        const Bdd& unusedVar = vars[((int)ceil(log2(maxDecisions))) - 1];
        for(auto& v : leftPaths) {
            v &= unusedVar;
        }
        for(auto& v : rightPaths) {
            v &= !unusedVar;
        }
        leftPaths.insert(leftPaths.end(), rightPaths.begin(), rightPaths.end());
        return leftPaths;
    }
}

inline Int
ExecutionEngine::randomInt(const Int numChoices) {
#ifdef USE_VALUE_SUMMARY
    static int decisionCount = 0;
	int maxChoice = numChoices.maxValue();
    int numDecisionVars = (int)ceil(log2(maxChoice));    
    Bdd allVars[numDecisionVars];
    for(int i = 0; i < numDecisionVars; i++) {
        allVars[i] = newBoolVar("decision_" + std::to_string(decisionCount++) + "_" + std::to_string(i));
    }
    Int::Builder builder;
    for(auto& gv : numChoices.values) {
        std::vector<Bdd>&& allPaths = buildTreePaths(allVars, gv.first);
        for(int i = 0; i < gv.first; i++) {
            builder.addValue(gv.second & allPaths[i], i);
        }
    }
    return builder.build();
#else
    return rand() % numChoices;
#endif
}

};

#endif