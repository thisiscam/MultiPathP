#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "SendQueueItem.hpp"

namespace RUNTIME_NAMESPACE {

struct SchedulerChoice {
public:
    SchedulerChoice() = default;

    SchedulerChoice(const Ptr<PMachine>& machine, const Int& queueIdx, const Int& stateIdx) noexcept:
        machine(machine),queueIdx(queueIdx),stateIdx(stateIdx) { }

    SchedulerChoice(Ptr<PMachine>&& machine, Int&& queueIdx, Int&& stateIdx) noexcept:
        machine(std::move(machine)),
        queueIdx(std::move(queueIdx)),
        stateIdx(std::move(stateIdx)) 
    {
         
    }

    Ptr<PMachine> machine;
    Int queueIdx;
    Int stateIdx;

#ifdef USE_VALUE_SUMMARY
    class Builder {
        Ptr<PMachine>::Builder machine;
        Int::Builder queueIdx;
        Int::Builder stateIdx;
    public:
        inline Builder& addValue(const Bdd& pred, const SchedulerChoice& rhs) {
            machine.addValue(pred, rhs.machine);
            queueIdx.addValue(pred, rhs.queueIdx);
            stateIdx.addValue(pred, rhs.stateIdx);
            return *this;
        }

        inline SchedulerChoice build() {
            return SchedulerChoice(machine.build(), queueIdx.build(), stateIdx.build());
        }
    };
#endif
};

class Scheduler {

protected:

    virtual SchedulerChoice chooseMachine() = 0;

    static inline const PList<SendQueueItem>& getSendQueue(PMachine* machine);
    
    ExecutionEngine& engine;

public:
    Scheduler(ExecutionEngine& engine):engine(engine) { }

    virtual void addMachine(const Ptr<PMachine>& machine, const PAny& payload = PAny::Null()) = 0;

    inline Bool step();

private:
    SendQueueItem popSendQueueItem(const Ptr<PMachine>& machine, const Int& index);
};

};

#include "Scheduler.ipp"

#endif