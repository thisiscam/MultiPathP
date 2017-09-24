#ifndef DELAY_BOUNDED_SCHEDULER
#define DELAY_BOUNDED_SCHEDULER

#include "Scheduler.hpp"

namespace RUNTIME_NAMESPACE {

class PMachine;

class DelayBoundedScheduler final : public Scheduler {

private:

	PList<Ptr<PMachine>> machines;

	static Ptr<const PList<SendQueueItem>> getSendQueuePtr(const Ptr<PMachine>& machine);

	Int currentMachineIndex = 0;
	Int delayBudget = 0;

protected:

    inline SchedulerChoice chooseMachine() override;

    inline void addMachine(const Ptr<PMachine>& machine, const PAny& payload) override;

public:
    
    DelayBoundedScheduler(ExecutionEngine& engine, const Int& delayBudget = 0):
    	Scheduler(engine),
    	delayBudget(delayBudget) 
    { }

};

};

#include "DelayBoundedScheduler.ipp"

#endif