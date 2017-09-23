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
    
    void createMachine(const Ptr<PMachine>& machine) {
    	newMachinesBuffer.add(machine);
    }

    void transferNewMachines(Scheduler& scheduler);

private:
    int maxIteration;

    Set<PMachine*> newMachinesBuffer;
};

};

#include "ExecutionEngine.ipp"

#endif