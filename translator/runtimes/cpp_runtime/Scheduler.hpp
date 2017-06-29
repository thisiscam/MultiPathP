#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "SendQueueItem.hpp"

namespace RUNTIME_NAMESPACE {

struct SchedulerChoice {
public:
    SchedulerChoice() = default;

    SchedulerChoice(const Ptr<PMachine>& machine, const Int& queueIdx, const Int& stateIdx):
        machine(machine),queueIdx(queueIdx),stateIdx(stateIdx) { }

    SchedulerChoice(Ptr<PMachine>&& machine, Int&& queueIdx, Int&& stateIdx):
        machine(std::move(machine)),
        queueIdx(std::move(queueIdx)),
        stateIdx(std::move(stateIdx)) 
    { }

    Ptr<PMachine> machine;
    Int queueIdx;
    Int stateIdx;

    class Builder {
        Ptr<PMachine>::Builder machine;
        Int::Builder queueIdx;
        Int::Builder stateIdx;
    public:
        inline Builder& addValue(const Bdd& pred, SchedulerChoice&& rhs) {
            machine.addValue(pred, std::move(rhs.machine));
            queueIdx.addValue(pred, std::move(rhs.queueIdx));
            stateIdx.addValue(pred, std::move(rhs.stateIdx));
            return *this;
        }

        inline SchedulerChoice build() {
            return SchedulerChoice(machine.build(), queueIdx.build(), stateIdx.build());
        }
    };
};

class Scheduler {

protected:

    virtual SchedulerChoice chooseMachine() = 0;

    inline PList<SendQueueItem>& getSendQueue(PMachine* machine);
    
    ExecutionEngine& engine;

public:
    Scheduler(ExecutionEngine& engine):engine(engine) { }

    virtual void startMachine(const Ptr<PMachine>& machine, const PAny& payload = PAny()) = 0;

    inline Bool step();

private:
    SendQueueItem popSendQueueItem(const Ptr<PMachine>& machine, Int index);
};

};

#include "Scheduler.ipp"

#endif