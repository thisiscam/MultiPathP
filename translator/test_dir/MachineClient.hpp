namespace pingpong {

class MachineClient final : public PMachine {
    friend class Scheduler;
public:
    MachineClient(ExecutionEngine& engine):PMachine(engine) { }

    inline void start(const PAny& payload) override {
        InitEntry(this, payload);
    }

private:
    enum {
        Init = 1,
        DoWrite = 2,
        DoRead = 3,
        End = 4
    };

    /* region Entry Methods */
    static inline void InitEntry(MachineClient* self, const PAny& payload) {
        self->states.setTop(Init);
        self->InitEntryImpl(payload);
    }

    static inline void DoWriteEntry(MachineClient* self, const PAny& payload) {
        self->states.setTop(DoWrite);
        self->DoWriteEntryImpl();
    }

    static inline void DoReadEntry(MachineClient* self, const PAny& payload) {
        self->states.setTop(DoRead);
        self->DoReadEntryImpl();
    }

    static inline void EndEntry(MachineClient* self, const PAny& payload) {
        self->states.setTop(End);
    }
    /* end Entry Methods */

    /* region Transition Methods */
    /* end Transition Methods */

    /* region Function Implementations */
    inline VOID_FUNCTION_DECL(InitEntryImpl, (Ptr<PMachine> payload)) {
        coordinator = payload;
        raise(Unit); retcode = RAISED_EVENT; RETURN_VOID();
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(DoWriteEntryImpl, ()) {
        idx = ChooseIndex();
        val = ChooseValue();
        send(coordinator, WRITE_REQ, PTuple<Ptr<PMachine>, Int, Int>(self(), idx, val));
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(DoReadEntryImpl, ()) {
        send(coordinator, READ_REQ, PTuple<Ptr<PMachine>, Int>(self(), idx));
    }
    END_VOID_FUNCTION()

    inline FUNCTION_DECL(ESC(Int), ChooseIndex, ()) {
        IF(randomBool("3")) 
        THEN() {
            RETURN(0);
        }
        ELSE() {
            RETURN(1);
        }
        ENDIF()
    }
    END_FUNCTION()

    inline FUNCTION_DECL(ESC(Int), ChooseValue, ()) {
        IF(randomBool("4")) 
        THEN() {
            RETURN(0);
        }
        ELSE() {
            RETURN(1);
        }
        ENDIF()
    }
    END_FUNCTION()
    /* end Function Implementations */

    /* region Machine Fields */
    Ptr<PMachine> coordinator;
    Int idx;
    Int val;
    /* end Machine Fields  */

    /* region Jump Tables */
    inline Bool isDefered(const Int& state, const Int& event) const override {
        static const bool _isDefered[5][22] = 
            {
                { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true} /* halt */,
                { true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* Init */,
                { true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* DoWrite */,
                { true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* DoRead */,
                { true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* End */
            };
        return getIndex2D(_isDefered, state, event);
    }

    inline Bool isGotoTransition(const Int& state, const Int& event) const override {
        static const bool _isGotoTransition[5][22] =
            {
                {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* halt */,
                {false, true,false,false,false,false,false,false,false,false,false,false,false, true,false,false,false,false,false,false,false,false} /* Init */,
                {false, true,false,false,false,false,false,false, true, true,false,false,false,false,false,false,false,false,false,false,false,false} /* DoWrite */,
                {false, true,false,false,false,false,false,false,false,false,false, true, true,false,false,false,false,false,false,false,false,false} /* DoRead */,
                {false, true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* End */
            };
        return getIndex2D(_isGotoTransition, state, event);
    }

    inline ExitFunctionPtr getExitFunction(const Int& state) const override {
        #define E(f) ((ExitFunction)&MachineClient::f)
        static ExitFunction _exitFunctions[] = {&MachineClient::emptyExit,&MachineClient::emptyExit,&MachineClient::emptyExit,&MachineClient::emptyExit,&MachineClient::emptyExit};
        #undef E
        return getIndex1D(_exitFunctions, state);
    }

    inline TransitionFunctionPtr getTransition(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineClient::f)
        static TransitionFunction _transitions[5][22] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,E(emptyTransition),E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(emptyTransition),E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}
            };
        #undef E
        return getIndex2D(_transitions, state, event);
    }

    inline EntryFunctionPtr getTransitionEntry(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineClient::f)
        static TransitionFunction _entries[5][22] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(DoWriteEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,NULL,NULL,NULL,NULL,E(EndEntry),E(DoReadEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(EndEntry),E(EndEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}
            };
        #undef E
        return getIndex2D(_entries, state, event);
    }
    /* end Jump Tables */
};

};