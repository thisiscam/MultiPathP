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

    using TransitionFunction = void (*)(PMachine*, const PAny& any);
    using EntryFunction  = void (*)(PMachine*, const PAny& any);
    using ExitFunction = void (*)(PMachine*);

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

    inline FUNCTION_DECL(Int, canServeEvent, (Int e)) {
        FOR(Int i = states.size() - 1, i >= 0, --i, {
            Int state = states.get(i);
            IF(isDefered(state, e))
            THEN() {
                RETURN(-1);
            }
            ELSE() {
                IF(getTransition(state, e) != nullptr) 
                THEN() {
                    RETURN(i);
                }
                ENDIF()
            }
            ENDIF()
        })
        ENDFOR()
        throw runtime_error("Unhandled event");
    }
    END_FUNCTION()

    inline VOID_FUNCTION_DECL(step, (Int stateIndex, Int e, const PAny& payload = PAny())) {
        Int state = states.get(stateIndex);
        IF(isGotoTransition(state, e))
        THEN() {
            FOR(Int i = states.size() - 1, i > stateIndex, --i, {
                popState();
            })
            ENDFOR_NC()
        }
        ENDIF()
        retcode = EXECUTE_FINISHED;
        TransitionFunctionPtr transitionFn = getTransition(state, e);
        INVOKE_PTR(transitionFn, void, (this, payload));
        EntryFunctionPtr entryFn = getTransitionEntry(state, e);
        INVOKE_PTR(entryFn, void, (this, payload));
    }
    END_VOID_FUNCTION()

protected:

    inline Ptr<PMachine> self() {
        return this;
    }

    inline void send(const Ptr<PMachine>& other, Int e, const PAny& payload = PAny()) {
        sendQueue.add(SendQueueItem(other, e, payload));
    }

    template<typename M>
    Ptr<PMachine> create(const PAny& payload = PAny()) {
        Ptr<PMachine> machine = PMachine::alloc<M>(engine);
        sendQueue.add(SendQueueItem(machine, EVENT_NEW_MACHINE, payload));
        return machine;
    }

    inline VOID_FUNCTION_DECL(raise, (Int e, const PAny& payload = PAny())) {
        FOR(Int i = states.size() - 1, i >= 0, --i, {
            Int state = states.get(i);
            TransitionFunctionPtr f = getTransition(state, e);
            IF(f != nullptr)
            THEN() {
                INVOKE_PTR(f, void, (this, payload));
                EntryFunctionPtr entryFn = getTransitionEntry(state, e);
                INVOKE_PTR(entryFn, void, (this, payload));
                RETURN();
            }
            ELSE() {
                popState();
            }
            ENDIF()
        })
        ENDFOR()
        PathConstraint::printDot("pc");
        throw runtime_error("Unhandled event");
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(popState, ()) {
        Int last = states.size() - 1;
        Int current_state = states.get(last);
        states.removeRange(last);
        ExitFunctionPtr eF = getExitFunction(current_state);
        IF(eF != nullptr) 
        THEN() {
            INVOKE_PTR(eF, void, (this));
        }
        ENDIF()
    }
    END_VOID_FUNCTION()

    inline Bool randomBool(const std::string&);

    inline void passert(Bool cond, const string& message) {
        IF_ONLY(!cond) {
            throw runtime_error(message);
        }
    }

    static inline void emptyTransition(PMachine* self, const PAny& payload) { }

    static inline void transitionPushState(PMachine* self, const PAny& payload) {
        ++self->states._size;
    }

    static inline void emptyEntry(PMachine* self, const PAny& payload) { }

    static inline void haltEntry(PMachine* self, const PAny& payload) {
        self->states = PList<Int>();
        self->states.add(STATE_HALT);
    }

    static inline void emptyExit(PMachine* self) { }

    Int retcode;
    PList<Int> states;

    PList<SendQueueItem> sendQueue;

private:
    ExecutionEngine& engine;

    virtual Bool isDefered(const Int& state, const Int& event) const = 0;
    virtual Bool isGotoTransition(const Int& state, const Int& event) const = 0;
    virtual ExitFunctionPtr getExitFunction(const Int& state) const = 0;
    virtual TransitionFunctionPtr getTransition(const Int& state, const Int& event) const = 0;
    virtual EntryFunctionPtr getTransitionEntry(const Int& state, const Int& event) const = 0;

    static std::map<const type_info*, Allocator<PMachine>*>& allocators() { 
        static std::map<const type_info*, Allocator<PMachine>*> allocators;
        return allocators;
    }

public:
    template<typename M>
    static Ptr<PMachine> alloc(ExecutionEngine& engine) {
        if(allocators().count(&typeid(M)) == 0) {
            //TODO better engine logic 
            allocators().insert({&typeid(M), Allocator<PMachine>::create([&](int count) { return new M(engine); })});
        }
        return allocators().at(&typeid(M))->allocate();
    }
};

};

#include "PMachine.ipp"

#endif