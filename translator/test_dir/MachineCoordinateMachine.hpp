namespace pingpong {

class MachineCoordinateMachine final : public PMachine {
    friend class Scheduler;
public:
    MachineCoordinateMachine(ExecutionEngine& engine):PMachine(engine) { }

    inline void start(const PAny& payload) override {
        InitEntry(this, payload);
    }

private:
    enum {
        Init = 1,
        WaitForRequest = 2,
        TransactionState = 3
    };

    /* region Entry Methods */
    static inline void InitEntry(MachineCoordinateMachine* self, const PAny& payload) {
        self->states.setTop(Init);
        self->InitEntryImpl();
    }

    static inline void WaitForRequestEntry(MachineCoordinateMachine* self, const PAny& payload) {
        self->states.setTop(WaitForRequest);
    }

    static inline void TransactionStateEntry(MachineCoordinateMachine* self, const PAny& payload) {
        self->states.setTop(TransactionState);
        self->TransactionStateEntryImpl();
    }
    /* end Entry Methods */

    /* region Transition Methods */
    /* end Transition Methods */

    /* region Function Implementations */
    inline VOID_FUNCTION_DECL(InitEntryImpl, ()) {
        participant = create<MachineParticipantMachine>(self());
        raise(eUnit); retcode = RAISED_EVENT; RETURN();
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(TransactionStateEntryImpl, ()) {
        send(participant, eCommit);
    }
    END_VOID_FUNCTION()
    /* end Function Implementations */

    /* region Machine Fields */
    Ptr<PMachine> participant;
    /* end Machine Fields  */

    /* region Jump Tables */
    inline Bool isDefered(const Int& state, const Int& event) const override {
        static const bool _isDefered[4][7] = 
            {
                { true, true, true, true, true, true, true} /* halt */,
                { true,false,false,false,false,false,false} /* Init */,
                { true,false,false,false,false,false,false} /* WaitForRequest */,
                { true,false,false, true,false,false,false} /* TransactionState */
            };
        return getIndex2D(_isDefered, state, event);
    }

    inline Bool isGotoTransition(const Int& state, const Int& event) const override {
        static const bool _isGotoTransition[4][7] =
            {
                {false,false,false,false,false,false,false} /* halt */,
                {false, true, true,false,false,false,false} /* Init */,
                {false, true,false, true,false,false,false} /* WaitForRequest */,
                {false, true,false,false,false, true, true} /* TransactionState */
            };
        return getIndex2D(_isGotoTransition, state, event);
    }

    inline ExitFunctionPtr getExitFunction(const Int& state) const override {
        #define E(f) ((ExitFunction)&MachineCoordinateMachine::f)
        static ExitFunction _exitFunctions[] = {&MachineCoordinateMachine::emptyExit,&MachineCoordinateMachine::emptyExit,&MachineCoordinateMachine::emptyExit,&MachineCoordinateMachine::emptyExit};
        #undef E
        return getIndex1D(_exitFunctions, state);
    }

    inline TransitionFunctionPtr getTransition(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineCoordinateMachine::f)
        static TransitionFunction _transitions[4][7] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),E(emptyTransition),NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,E(emptyTransition),NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,NULL,E(emptyTransition),E(emptyTransition)}
            };
        #undef E
        return getIndex2D(_transitions, state, event);
    }

    inline EntryFunctionPtr getTransitionEntry(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineCoordinateMachine::f)
        static TransitionFunction _entries[4][7] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),E(WaitForRequestEntry),NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,E(TransactionStateEntry),NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,NULL,E(WaitForRequestEntry),E(WaitForRequestEntry)}
            };
        #undef E
        return getIndex2D(_entries, state, event);
    }
    /* end Jump Tables */
};

};