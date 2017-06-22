#ifndef PMACHINE_HPP
#define PMACHINE_HPP

#include <vector>
#include <stdexcept>
#include <string>
#include <typeinfo>

#include "PList.hpp"
#include "PAny.hpp"
#include "PTuple.hpp"
#include "SendQueueItem.hpp"
#include "Allocator.hpp"
#include "Helpers.h"

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

#ifdef USE_VALUE_SUMMARY
    using TransitionFunctionPtr = ValueSummary<TransitionFunction>;
    using EntryFunctionPtr  = ValueSummary<EntryFunction>;
    using ExitFunctionPtr = ValueSummary<ExitFunction>;
#else
    using TransitionFunctionPtr = TransitionFunction;
    using EntryFunctionPtr  = EntryFunction;
    using ExitFunctionPtr = ExitFunction;
#endif

public:
    PMachine(ExecutionEngine& engine):engine(engine) { 
        states.add(STATE_HALT);
    }

    virtual void start(const PAny& payload) = 0;

    inline FUNCTION_DECL(Int, canServeEvent, (Int e), {
        FOR(Int i = states.size() - 1, i >= 0, --i, {
            Int state = states.get(i);
            IF(isDefered(state, e)) 
            THEN({
                RETURN(-1);
            }) 
            ELSE({
                IF(getTransition(state, e) != NULL) 
                THEN({
                    RETURN(i);
                })
                ENDIF()
            })
            ENDIF()
        })
        ENDFOR()
        throw runtime_error("Unhandled event");
    })

    inline FUNCTION_DECL(void, step, (Int stateIndex, Int e, const PAny& payload = PAny()), {
        Int state = states.get(stateIndex);
        IF(isGotoTransition(state, e)) 
        THEN({
            FOR(Int i = states.size() - 1, i > stateIndex, --i, {
                popState();
            })
            ENDFOR_NC()
        })
        ENDIF()
        retcode = EXECUTE_FINISHED;
        TransitionFunctionPtr transitionFn = getTransition(state, e);
        INVOKE_PTR_ON_THIS(transitionFn, void, (payload));
        EntryFunctionPtr entryFn = getTransitionEntry(state, e);
        INVOKE_PTR_ON_THIS(entryFn, void, (payload));
    })

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

    inline FUNCTION_DECL(void, raise, (Int e, const PAny& payload = PAny()), {
        FOR(Int i = states.size() - 1, i >= 0, --i, {
            Int state = states.get(i);
            TransitionFunctionPtr f = getTransition(state, e);
            IF(f != NULL)
            THEN({
                INVOKE_PTR_ON_THIS(f, void, (payload));
                EntryFunctionPtr entryFn = getTransitionEntry(state, e);
                INVOKE_PTR_ON_THIS(entryFn, void, (payload));
                RETURN();
            })
            ELSE({
                popState();
            })
            ENDIF()
        })
        ENDFOR()
        throw runtime_error("Unhandled event");
    })

    inline FUNCTION_DECL(void, popState, (), {
        Int last = states.size() - 1;
        Int current_state = states.get(last);
        states.removeRange(last);
        ExitFunctionPtr eF = getExitFunction(current_state);
        IF(eF != NULL) 
        THEN({
            INVOKE_PTR_ON_THIS(eF, void, ());
        })
        ENDIF()
    })

    inline Bool randomBool(const std::string&);

    inline void passert(Bool cond, const string& message) {
        IF_ONLY(!cond) {
            throw runtime_error(message);
        }
    }

    inline void emptyTransition(const PAny& payload) { }

    inline void transitionPushState(const PAny& payload) {
        ++states._size;
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
    virtual ExitFunctionPtr getExitFunction(Int state) const = 0;
    virtual TransitionFunctionPtr getTransition(Int state, Int event) const = 0;
    virtual EntryFunctionPtr getTransitionEntry(Int state, Int event) const = 0;

    static std::map<const type_info*, Allocator<Ptr<PMachine>>>& allocators() { 
        static std::map<const type_info*, Allocator<Ptr<PMachine>>> allocators;
        return allocators;
    }

    template<typename M>
    static Ptr<PMachine> alloc(ExecutionEngine& engine) {
        if(allocators().count(&typeid(M)) == 0) {
            //TODO better engine logic 
            allocators().insert({&typeid(M), Allocator<Ptr<PMachine>>([&]() { return new M(engine); })});
        }
        return allocators().at(&typeid(M)).allocate();
    }
};

};

#include "PMachine.ipp"

#endif