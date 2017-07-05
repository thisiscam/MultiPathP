#ifndef EXECUTION_ENGINE_HPP
#define EXECUTION_ENGINE_HPP

#include <string>

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
    
private:
    int maxIteration;
};

};

#include "ExecutionEngine.ipp"

#endif