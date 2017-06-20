#ifndef PMACHINE_HPP
#define PMACHINE_HPP

#include <vector>
#include <stdexcept>
#include <string>

#include "PList.hpp"
#include "PAny.hpp"
#include "PTuple.hpp"
#include "SendQueueItem.hpp"

using namespace std;

namespace RUNTIME_NAMESPACE {

const int EXECUTE_FINISHED = 0;
const int RAISED_EVENT = 1;

const int EVENT_NEW_MACHINE = -1;
const int EVENT_NULL = 0;
const int EVENT_HALT = 1;

const int STATE_HALT = 0;

class ExecutionEngine;

class PMachine
{
    friend class Scheduler;

protected:

    using TransitionFunction = void (PMachine::*)(const PAny& any);
    using EntryFunction  = void (PMachine::*)(const PAny& any);
    using ExitFunction = void (PMachine::*)();

public:
    PMachine(ExecutionEngine& engine):engine(engine) { 
        states.add(STATE_HALT);
    }

    virtual void start(const PAny& payload) = 0;

    inline Int canServeEvent(Int e) {
        for(Int i = states.size() - 1; i >= 0; i--) {
            Int state = states.get(i);
            if(isDefered(state, e)) {
                return -1;
            } else if(getTransition(state, e) != NULL) {
                return i;
            }
        }
        throw runtime_error("Unhandled event");
    }

    inline void step(Int stateIndex, Int e, const PAny& payload = PAny()) {
        Int state = states.get(stateIndex);
        if(isGotoTransition(state, e)) {
            for(Int i = states.size() - 1; i > stateIndex; i--) {
                popState();
            }
        }
        retcode = EXECUTE_FINISHED;
        TransitionFunction transitionFn = getTransition(state, e);
        (this->*transitionFn)(payload);
        EntryFunction entryFn = getTransitionEntry(state, e);
        (this->*entryFn)(payload);
    }

protected:

    inline void send(Ptr<PMachine> other, Int e, const PAny& payload = PAny()) {
        sendQueue.add(SendQueueItem(other, e, payload));
    }

    template<typename M>
    Ptr<PMachine> create(const PAny& payload = PAny()) {
        Ptr<PMachine> machine = PMachine::alloc<M>(engine);
        sendQueue.add(SendQueueItem(machine, EVENT_NEW_MACHINE, payload));
        return machine;
    }

    inline void raise(Int e, const PAny& payload = PAny()) {
        for(Int i = states.size() - 1; i >= 0; i--) {
            Int state = states.get(i);
            TransitionFunction f = getTransition(state, e);
            if(f != NULL) {
                (this->*f)(payload);
                EntryFunction entryFn = getTransitionEntry(state, e);
                (this->*entryFn)(payload);
                return;
            } else {
                popState();
            }
        }
        throw runtime_error("Unhandled event");
    }

    inline void popState() {
        Int last = states.size() - 1;
        Int current_state = states.get(last);
        states.removeRange(last);
        ExitFunction eF = getExitFunction(current_state);
        if(eF != NULL) {
            (this->*eF)();
        }
    }

    inline Bool randomBool();

    inline void passert(Bool cond, const string& message) {
        if(!cond) {
            throw runtime_error(message);
        }
    }

    inline void emptyTransition(const PAny& payload) { }

    inline void transitionPushState(const PAny& payload) {
        states._size ++;
    }

    inline void emptyEntry(const PAny& payload) { }

    inline void haltEntry(const PAny& payload) {
        states = PList<Int>();
        states.add(STATE_HALT);
    }

    inline void emptyExit() { }

    Int retcode;
    PList<Int> states;

private:
    ExecutionEngine& engine;

    PList<SendQueueItem> sendQueue;

    virtual Bool isDefered(Int state, Int event) const = 0;
    virtual Bool isGotoTransition(Int state, Int event) const = 0;
    virtual ExitFunction getExitFunction(Int state) const = 0;
    virtual TransitionFunction getTransition(Int state, Int event) const = 0;
    virtual EntryFunction getTransitionEntry(Int state, Int event) const = 0;

    template<typename M>
    static Ptr<PMachine> alloc(ExecutionEngine& engine) {
        return new M(engine);
    }
};

};

#include "PMachine.ipp"

#endif