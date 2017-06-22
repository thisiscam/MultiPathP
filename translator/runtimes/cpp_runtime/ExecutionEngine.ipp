#ifndef EXECUTION_ENGINE_IPP
#define EXECUTION_ENGINE_IPP

#include <map>
#include <string>
#include <list>
#include <tuple>
#include "Scheduler.hpp"

namespace RUNTIME_NAMESPACE {

static Bool newBoolVar(const std::string& id) {
	static std::list<std::tuple<const std::string, Bool>> allocatedBools;
#ifdef USE_VALUE_SUMMARY
	Bdd T = Bdd((uint32_t)allocatedBools.size());
	ValueSummary<bool>::Builder builder;
	builder.addValue(T, true).addValue(!T, false);
	Bool ret = builder.build();
#else
	Bool ret = rand() % 2 == 0;
#endif
	allocatedBools.push_back(std::make_tuple(id, ret));
	return ret;
}

inline void 
ExecutionEngine::run(Scheduler& scheduler, Ptr<PMachine> machine) {
    scheduler.startMachine(machine);
    for(int i = 0; i < maxIteration; ++i) {
        IF_ONLY(!scheduler.step()) {
            break;
        }
    }
}

inline Bool
ExecutionEngine::randomBool(const std::string& id) {
	static std::map<const std::string, Allocator<Bool>> allocators;
	if(allocators.count(id) == 0) {
		allocators.insert({id, Allocator<Bool>([&](){ 
			static int count = 0;
			return newBoolVar(id + std::to_string(count++)); 
		})});
	}
	return allocators.at(id).allocate();
}

inline Int
ExecutionEngine::randomInt(Int max) {
#ifdef USE_VALUE_SUMMARY
#else
    return rand() % max;
#endif
}

};

#endif