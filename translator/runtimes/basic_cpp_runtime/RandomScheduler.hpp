#ifndef RANDOM_SCHEDULER
#define RANDOM_SCHEDULER

#include "Scheduler.hpp"

namespace basic_cpp_runtime {

class PMachine;

class RandomScheduler final : public Scheduler {

protected:

    inline SchedulerChoice chooseMachine() override;

    inline void startMachine(PMachine* machine, const PAny& payload) override;


public:
    
    RandomScheduler(ExecutionEngine& engine):Scheduler(engine) { }

};

};

#include "RandomScheduler.ipp"

#endif