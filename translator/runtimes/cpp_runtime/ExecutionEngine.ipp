#ifndef EXECUTION_ENGINE_IPP
#define EXECUTION_ENGINE_IPP

#include <map>
#include <string>
#include <list>
#include <tuple>
#include "Scheduler.hpp"

namespace RUNTIME_NAMESPACE {

inline void 
ExecutionEngine::run(Scheduler& scheduler, Ptr<PMachine> machine) {
    scheduler.startMachine(machine);
    for(int i = 0; i < maxIteration; ++i) {
    	std::cout << "======== Step " << std::to_string(i) << "=======" << std::endl;
        IF_ONLY(!scheduler.step()) {
            break;
        }
    }
}

inline Bool
ExecutionEngine::randomBool(const std::string& id) {
	static std::map<const std::string, Allocator<bool>*> allocators;
	if(allocators.count(id) == 0) {
		allocators.insert({id, Allocator<bool>::create(id)});
	}
	Bool&& ret = allocators.at(id)->allocate();
	return ret;
}

inline Int
ExecutionEngine::randomInt(Int max) {
#ifdef USE_VALUE_SUMMARY
	return 0;
#else
    return rand() % max;
#endif
}

};

#endif