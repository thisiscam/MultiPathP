#ifndef RANDOM_SCHEDULER
#define RANDOM_SCHEDULER

#include "Scheduler.hpp"

namespace RUNTIME_NAMESPACE {

class PMachine;

class RandomScheduler final : public Scheduler {

protected:

    inline SchedulerChoice chooseMachine() override;

    inline void startMachine(Ptr<PMachine> machine, const PAny& payload) override;


public:
    
    RandomScheduler(ExecutionEngine& engine):Scheduler(engine) { }

};

};

#include "RandomScheduler.ipp"

#endif