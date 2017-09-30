#ifndef EXECUTION_ENGINE_HPP
#define EXECUTION_ENGINE_HPP

#include <string>
#include "Set.hpp"

namespace RUNTIME_NAMESPACE {

class Scheduler;

class ExecutionEngine {
public:
    ExecutionEngine(int maxIteration=200):
        maxIteration(maxIteration)
    { }

    inline void run(Scheduler& scheduler, Ptr<PMachine> machine);

    Bool randomBool(const std::string& id);

    Int randomInt(const Int max);
    
    void createMachine(const Ptr<PMachine>& machine);

#ifdef USE_VALUE_SUMMARY
    template<typename T>
	void incrementTransitionCount(T&& v) {
		transitionCount += v.values.size();
	}
#else
	template<typename T>
	void incrementTransitionCount(T && v) {
		transitionCount += 1;
	}
#endif

    inline int getTransitionCount() {
    	return transitionCount;
    }

private:
    int maxIteration;
    Scheduler* scheduler;

    int transitionCount;
};

};

#include "ExecutionEngine.ipp"

#endif