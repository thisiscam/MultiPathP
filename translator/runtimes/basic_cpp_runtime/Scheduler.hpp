#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "SendQueueItem.hpp"

namespace basic_cpp_runtime {

struct SchedulerChoice {
public:
    SchedulerChoice() = default;

    SchedulerChoice(PMachine* machine, const int& queueIdx, const int& stateIdx):
        machine(machine),queueIdx(queueIdx),stateIdx(stateIdx) { }

    PMachine* machine;
    int queueIdx;
    int stateIdx;
};

class Scheduler {

protected:

    virtual SchedulerChoice chooseMachine() = 0;

    inline PList<SendQueueItem>& getSendQueue(PMachine* machine);

    PList<PMachine*> machines;
    
    ExecutionEngine& engine;

public:
    Scheduler(ExecutionEngine& engine):engine(engine) { }

    virtual void startMachine(PMachine* machine, const PAny& payload = PAny()) = 0;

    inline bool step();

};

};

#include "Scheduler.ipp"

#endif