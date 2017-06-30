namespace pingpong {

class MachineTimer final : public PMachine {
    friend class Scheduler;
public:
    MachineTimer(ExecutionEngine& engine):PMachine(engine) { }

    inline void start(const PAny& payload) override {
        InitEntry(this, payload);
    }

private:
    enum {
        Init = 1,
        Loop = 2,
        TimerStarted = 3
    };

    /* region Entry Methods */
    static inline void InitEntry(MachineTimer* self, const PAny& payload) {
        self->states.setTop(Init);
        self->InitEntryImpl(payload);
    }

    static inline void LoopEntry(MachineTimer* self, const PAny& payload) {
        self->states.setTop(Loop);
    }

    static inline void TimerStartedEntry(MachineTimer* self, const PAny& payload) {
        self->states.setTop(TimerStarted);
        self->TimerStartedEntryImpl();
    }
    /* end Entry Methods */

    /* region Transition Methods */
    /* end Transition Methods */

    /* region Function Implementations */
    inline VOID_FUNCTION_DECL(InitEntryImpl, (PTuple<Ptr<PMachine>, Int> payload)) {
        target = payload.v0();
        timeoutvalue = payload.v1();
        raise((local)); retcode = RAISED_EVENT; RETURN_VOID();
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(TimerStartedEntryImpl, ()) {
        IF(randomBool("0")) 
        THEN() {
            raise((local)); retcode = RAISED_EVENT; RETURN_VOID();
        }
        ENDIF()
    }
    END_VOID_FUNCTION()
    /* end Function Implementations */

    /* region Machine Fields */
    Ptr<PMachine> target;
    Int timeoutvalue;
    /* end Machine Fields  */

    /* region Jump Tables */
    inline Bool isDefered(const Int& state, const Int& event) const override {
        static const bool _isDefered[4][25] = 
            {
                { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true} /* halt */,
                { true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* Init */,
                { true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* Loop */,
                { true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* TimerStarted */
            };
        return getIndex2D(_isDefered, state, event);
    }

    inline Bool isGotoTransition(const Int& state, const Int& event) const override {
        static const bool _isGotoTransition[4][25] =
            {
                {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* halt */,
                {false, true,false,false,false,false,false, true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* Init */,
                {false, true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false, true,false,false,false} /* Loop */,
                {false, true,false,false,false,false,false, true,false,false,false,false,false,false,false,false,false,false,false,false,false,false, true,false,false} /* TimerStarted */
            };
        return getIndex2D(_isGotoTransition, state, event);
    }

    inline ExitFunctionPtr getExitFunction(const Int& state) const override {
        #define E(f) ((ExitFunction)&MachineTimer::f)
        static ExitFunction _exitFunctions[] = {&MachineTimer::emptyExit,&MachineTimer::emptyExit,&MachineTimer::emptyExit,&MachineTimer::emptyExit};
        #undef E
        return getIndex1D(_exitFunctions, state);
    }

    inline TransitionFunctionPtr getTransition(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineTimer::f)
        static TransitionFunction _transitions[4][25] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(emptyTransition),E(emptyTransition),NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(emptyTransition),E(emptyTransition),NULL,NULL}
            };
        #undef E
        return getIndex2D(_transitions, state, event);
    }

    inline EntryFunctionPtr getTransitionEntry(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineTimer::f)
        static TransitionFunction _entries[4][25] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,NULL,NULL,NULL,E(LoopEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(TimerStartedEntry),E(emptyEntry),NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,NULL,NULL,NULL,E(LoopEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(emptyEntry),E(LoopEntry),NULL,NULL}
            };
        #undef E
        return getIndex2D(_entries, state, event);
    }
    /* end Jump Tables */
};

};