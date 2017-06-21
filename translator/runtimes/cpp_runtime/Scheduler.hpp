#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "SendQueueItem.hpp"

namespace RUNTIME_NAMESPACE {

struct SchedulerChoice {
public:
    SchedulerChoice() = default;

    SchedulerChoice(Ptr<PMachine> machine, const Int& queueIdx, const Int& stateIdx):
        machine(machine),queueIdx(queueIdx),stateIdx(stateIdx) { }

    Ptr<PMachine> machine;
    Int queueIdx;
    Int stateIdx;
};

class Scheduler {

protected:

    virtual SchedulerChoice chooseMachine() = 0;

    inline PList<SendQueueItem>& getSendQueue(PMachine* machine);
    
    ExecutionEngine& engine;

public:
    Scheduler(ExecutionEngine& engine):engine(engine) { }

    virtual void startMachine(Ptr<PMachine> machine, const PAny& payload = PAny()) = 0;

    inline Bool step();

private:
    SendQueueItem popSendQueueItem(Ptr<PMachine> machine, Int index);
};

};

#include "Scheduler.ipp"

#endif