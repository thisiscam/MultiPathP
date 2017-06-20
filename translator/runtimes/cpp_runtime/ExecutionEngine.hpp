#ifndef EXECUTION_ENGINE_HPP
#define EXECUTION_ENGINE_HPP

namespace RUNTIME_NAMESPACE {

class Scheduler;

class ExecutionEngine {
public:
    ExecutionEngine(int maxIteration=200):
        maxIteration(maxIteration)
    { }

    inline void run(Scheduler& scheduler, Ptr<PMachine> machine);

    Bool randomBool();

    Int randomInt(Int max);
    
private:
    int maxIteration;
};

};

#include "ExecutionEngine.ipp"

#endif