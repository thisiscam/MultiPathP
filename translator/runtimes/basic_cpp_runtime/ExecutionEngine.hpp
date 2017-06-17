#ifndef EXECUTION_ENGINE_HPP
#define EXECUTION_ENGINE_HPP

namespace basic_cpp_runtime {

class Scheduler;

class ExecutionEngine {
public:
    ExecutionEngine(int maxIteration=200):
        maxIteration(maxIteration)
    { }

    inline void run(Scheduler& scheduler, PMachine* machine);

    bool randomBool();

    int randomInt(int max);
    
private:
    int maxIteration;
};

};

#include "ExecutionEngine.ipp"

#endif