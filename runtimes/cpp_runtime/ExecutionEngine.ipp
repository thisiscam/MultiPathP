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
    this->scheduler = &scheduler;
    scheduler.addMachine(machine);
    INVOKE(machine, void, start, ());
    for(int i = 0; i < maxIteration; ++i) {
    	std::cout << "======== BEGIN Step " << std::to_string(i) << "=======" << std::endl;
        IF_ONLY(scheduler.step()) {
            std::cout << "======== END Step " << std::to_string(i) << " transitions: " << getTransitionCount() << "=======" << std::endl;
            continue;
        } else {
            std::cout << "======== END Step " << std::to_string(i) << " transitions: " << getTransitionCount() << "=======" << std::endl;
            break;
        }
    }
}

inline void 
ExecutionEngine::createMachine(const Ptr<PMachine>& machine) {
    scheduler->addMachine(machine);
}

inline Bool
ExecutionEngine::randomBool(const std::string& id) {
	static std::map<const std::string, Allocator<bool>*> allocators;
	if(allocators.count(id) == 0) {
		allocators.insert({id, Allocator<bool>::create(id)});
	}
	return allocators.at(id)->allocate();
}

#ifdef USE_VALUE_SUMMARY
static inline
std::vector<Bdd> buildTreePaths(const vector<Bdd>& vars, const int maxDecisions)
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
#endif

inline Int
ExecutionEngine::randomInt(const Int numChoices) {
#ifdef USE_VALUE_SUMMARY
    static int decisionCount = 0;
	int maxChoice = numChoices.maxValue();
    int numDecisionVars = (int)ceil(log2(maxChoice));    
    std::vector<Bdd> allVars;
    for(int i = 0; i < numDecisionVars; i++) {
        allVars.push_back(newBoolVar("decision_" + std::to_string(decisionCount) + "_" + std::to_string(i)));
    }
    decisionCount++;
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