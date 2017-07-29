#ifndef PMACHINE_HPP
#define PMACHINE_HPP

#include <vector>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <iostream>
#include <unordered_map>

#include "PList.hpp"
#include "PAny.hpp"
#include "PTuple.hpp"
#include "SendQueueItem.hpp"
#include "Allocator.hpp"
#include "Helpers.h"

using namespace std;

struct pairhash {
public:
    template <typename T, typename U>
    std::size_t operator()(const std::pair<T, U> &x) const {
        return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
    }
};

namespace RUNTIME_NAMESPACE {

#define DECL_EVENT(e) inline const char* e () { static const char *_v = #e ; return _v; }
#define DECL_STATE(s) inline const char* s () { static const char *_v = #s ; return _v; }

const int EXECUTE_FINISHED = 0;
const int RAISED_EVENT = 1;

DECL_EVENT(EVENT_NEW_MACHINE);
DECL_EVENT(EVENT_NULL);
DECL_EVENT(EVENT_HALT);

DECL_STATE(STATE_HALT);

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

    using StateEventPair = std::pair<const char*, const char*>;
    using IsDeferedMapType = std::unordered_map<StateEventPair, bool, pairhash>;
    using IsGotoTransitionMapType = std::unordered_map<StateEventPair, bool, pairhash>;
    using GetExitFunctionMapType = std::unordered_map<const char*, ExitFunction>;
    using GetTransitionMapType = std::unordered_map<StateEventPair, TransitionFunction, pairhash>;
    using GetTransitionEntryMapType = std::unordered_map<StateEventPair, EntryFunction, pairhash>;

public:
    PMachine(ExecutionEngine& engine):
        engine(engine)
    {
        states.add(STATE_HALT);
    }

    virtual void start(const PAny& payload = PAny::Null()) = 0;

    inline FUNCTION_DECL(Int, canServeEvent, (const Int& e)) {
        FOR(Int i = states.size() - 1, i >= 0, --i, {
            const PState& state = states.get(i);
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

    inline VOID_FUNCTION_DECL(step, (const PState& stateIndex, const PEvent& e, const PAny& payload = PAny::Null())) {
        const PState& state = states.get(stateIndex);
        IF(isGotoTransition(state, e))
        THEN() {
            FOR(Int i = states.size() - 1, i > stateIndex, --i, {
                popState();
            })
            ENDFOR_NC()
        }
        ENDIF()
        retcode = EXECUTE_FINISHED;
        TransitionFunctionPtr&& transitionFn = getTransition(state, e);
        INVOKE_PTR(transitionFn, void, (this, payload));
        EntryFunctionPtr&& entryFn = getTransitionEntry(state, e);
        INVOKE_PTR(entryFn, void, (this, payload));
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(serveEvent, (const PEvent& e, const PAny& payload)) {
        FOR(Int i = 0, i < states.size(), ++i, {
            const PState& state = states.get(i);
            IF(!isDefered(state, e) & (getTransition(state, e) != nullptr)) 
            THEN() {
                IF(isGotoTransition(state, e))
                THEN() {
                    states.removeRange(0, i);
                }
                ENDIF()
                retcode = EXECUTE_FINISHED;
                TransitionFunctionPtr&& transition_fn = getTransition(state, e);
                INVOKE_PTR(transition_fn, void, (this, payload));
                EntryFunctionPtr&& entryFn = getTransitionEntry(state, e);
                INVOKE_PTR(entryFn, void, (this, payload));
                RETURN_VOID();
            }
            ENDIF()
        })
        ENDFOR()
        throw runtime_error("Unhandled event");
    }
    END_VOID_FUNCTION()

    const int id = idCount()++;

protected:

    inline Ptr<PMachine> self() {
        return this;
    }

    inline void send(const Ptr<PMachine>& other, const PEvent& e, const PAny& payload = PAny::Null()) {
        sendQueue.add(SendQueueItem(other, e, payload));
    }

    template<typename M>
    Ptr<PMachine> create(const PAny& payload = PAny::Null()) {
        Ptr<PMachine>&& machine = PMachine::alloc<M>(engine);
        sendQueue.add(SendQueueItem(machine, EVENT_NEW_MACHINE, payload));
        return machine;
    }

    inline VOID_FUNCTION_DECL(raise, (const PEvent& e, const PAny& payload = PAny::Null())) {
        FOR(Int i = states.size() - 1, i >= 0, --i, {
            const PState& state = states.get(i);
            TransitionFunctionPtr&& f = getTransition(state, e);
            IF(f != nullptr)
            THEN() {
                INVOKE_PTR(f, void, (this, payload));
                EntryFunctionPtr&& entryFn = getTransitionEntry(state, e);
                INVOKE_PTR(entryFn, void, (this, payload));
                RETURN_VOID();
            }
            ELSE() {
                popState();
            }
            ENDIF()
        })
        ENDFOR()
        throw runtime_error("Unhandled event");
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(popState, ()) {
        const Int& last = states.size() - 1;
        const Int& current_state = states.get(last);
        states.removeRange(last);
        ExitFunctionPtr&& eF = getExitFunction(current_state);
        IF(eF != nullptr) 
        THEN() {
            INVOKE_PTR(eF, void, (this));
        }
        ENDIF()
    }
    END_VOID_FUNCTION()

    inline Bool randomBool(const std::string&);

    inline void passert(const Bool& cond, const string& message) {
        IF_ONLY(!cond) {
            throw runtime_error(std::string("passert: [") + message + "]");
        }
    }

    static inline void emptyTransition(PMachine* self, const PAny& payload) { }

    static inline void pushTransition(PMachine* self, const PAny& payload) {
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

    inline Bool isDefered(const PState& state, const PEvent& event) const {
        return binaryOp<Bool>(state, event, [&](const char* s, const char* e) { 
            return isDeferedMap()[{s, e}];
        });
    }

    inline Bool isGotoTransition(const PState& state, const PEvent& event) const {
        return binaryOp<Bool>(state, event, [&](const char* s, const char* e) { 
            return isGotoTransitionMap()[{s, e}];
        });
    }

    ExitFunctionPtr getExitFunction(const PState& state) const {
        return unaryOp<ExitFunctionPtr>(state, [&](const char* s, const char* e) {
            return getExitFunctionMap()[{s, e}];
        });
    }

    TransitionFunctionPtr getTransition(const PState& state, const PEvent& event) const {
        return binaryOp<Bool>(state, event, [&](const char* s, const char* e) { 
            return getTransitionMap()[{s, e}];
        });
    }

    EntryFunctionPtr getTransitionEntry(const PState& state, const PEvent& event) const {
        return binaryOp<Bool>(state, event, [&](const char* s, const char* e) { 
            return getTransitionEntryMap()[{s, e}];
        });
    }

    virtual IsDeferedMapType isDeferedMap(const PState& state, const PEvent& event) const = 0;
    virtual IsGotoTransitionMapType isGotoTransitionMap(const PState& state, const PEvent& event) const = 0;
    virtual GetExitFunctionMapType getExitFunctionMap(const PState& state) const = 0;
    virtual GetTransitionMapType getTransitionMap(const PState& state, const PEvent& event) const = 0;
    virtual GetTransitionEntryMapType getTransitionEntryMap(const PState& state, const PEvent& event) const = 0;

    static std::map<const type_info*, Allocator<PMachine>*>& allocators() { 
        static std::map<const type_info*, Allocator<PMachine>*> allocators;
        return allocators;
    }

    static inline int& idCount() {
        static int idCnt = 0;
        return idCnt;
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

void operator<<(std::ostream& os, const PMachine& m) {
    os << typeid(m).name() << m.id;
}

};

#include "PMachine.ipp"

#endif