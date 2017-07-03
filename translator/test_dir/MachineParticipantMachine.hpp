namespace pingpong {

class MachineParticipantMachine final : public PMachine {
    friend class Scheduler;
public:
    MachineParticipantMachine(ExecutionEngine& engine):PMachine(engine) { }

    inline void start(const PAny& payload) override {
        InitEntry(this, payload);
    }

private:
    enum {
        Init = 1,
        WaitForRequest = 2
    };

    /* region Entry Methods */
    static inline void InitEntry(MachineParticipantMachine* self, const PAny& payload) {
        self->states.setTop(Init);
        self->InitEntryImpl(payload);
    }

    static inline void WaitForRequestEntry(MachineParticipantMachine* self, const PAny& payload) {
        self->states.setTop(WaitForRequest);
    }
    /* end Entry Methods */

    /* region Transition Methods */
    static inline void HandlerImpl0_Wrap(MachineParticipantMachine* self, const PAny& payload) {
        self->HandlerImpl0();
    }
    /* end Transition Methods */

    /* region Function Implementations */
    inline VOID_FUNCTION_DECL(InitEntryImpl, (Ptr<PMachine> payload)) {
        coor = payload;
        raise(eUnit); retcode = RAISED_EVENT; RETURN_VOID();
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(HandlerImpl0, ()) {
        IF(randomBool("3")) 
        THEN() {
            send(coor, eSuccess);
        }
        ELSE() {
            send(coor, eFailure);
        }
        ENDIF()
    }
    END_VOID_FUNCTION()
    /* end Function Implementations */

    /* region Machine Fields */
    Ptr<PMachine> coor;
    /* end Machine Fields  */

    /* region Jump Tables */
    inline Bool isDefered(const Int& state, const Int& event) const override {
        static const bool _isDefered[3][8] = 
            {
                { true, true, true, true, true, true, true, true} /* halt */,
                { true,false,false,false,false,false,false,false} /* Init */,
                { true,false,false,false,false,false,false,false} /* WaitForRequest */
            };
        return getIndex2D(_isDefered, state, event);
    }

    inline Bool isGotoTransition(const Int& state, const Int& event) const override {
        static const bool _isGotoTransition[3][8] =
            {
                {false,false,false,false,false,false,false,false} /* halt */,
                {false, true, true,false,false,false,false,false} /* Init */,
                {false, true,false,false,false,false,false,false} /* WaitForRequest */
            };
        return getIndex2D(_isGotoTransition, state, event);
    }

    inline ExitFunctionPtr getExitFunction(const Int& state) const override {
        #define E(f) ((ExitFunction)&MachineParticipantMachine::f)
        static ExitFunction _exitFunctions[] = {&MachineParticipantMachine::emptyExit,&MachineParticipantMachine::emptyExit,&MachineParticipantMachine::emptyExit};
        #undef E
        return getIndex1D(_exitFunctions, state);
    }

    inline TransitionFunctionPtr getTransition(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineParticipantMachine::f)
        static TransitionFunction _transitions[3][8] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),E(emptyTransition),NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,E(HandlerImpl0_Wrap),E(HandlerImpl0_Wrap),NULL,NULL}
            };
        #undef E
        return getIndex2D(_transitions, state, event);
    }

    inline EntryFunctionPtr getTransitionEntry(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineParticipantMachine::f)
        static TransitionFunction _entries[3][8] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),E(WaitForRequestEntry),NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,E(emptyEntry),E(emptyEntry),NULL,NULL}
            };
        #undef E
        return getIndex2D(_entries, state, event);
    }
    /* end Jump Tables */
};

};