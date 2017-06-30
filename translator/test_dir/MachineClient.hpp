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
        PumpRequestOne = 2,
        PumpRequestTwo = 3,
        Done = 4
    };

    /* region Entry Methods */
    static inline void InitEntry(MachineClient* self, const PAny& payload) {
        self->states.setTop(Init);
        self->InitEntryImpl(payload);
    }

    static inline void PumpRequestOneEntry(MachineClient* self, const PAny& payload) {
        self->states.setTop(PumpRequestOne);
        self->PumpRequestOneEntryImpl();
    }

    static inline void PumpRequestTwoEntry(MachineClient* self, const PAny& payload) {
        self->states.setTop(PumpRequestTwo);
        self->PumpRequestTwoEntryImpl();
    }

    static inline void DoneEntry(MachineClient* self, const PAny& payload) {
        self->states.setTop(Done);
    }
    /* end Entry Methods */

    /* region Transition Methods */
    /* end Transition Methods */

    /* region Function Implementations */
    inline VOID_FUNCTION_DECL(InitEntryImpl, (PList<Ptr<PMachine>> payload)) {
        servers = payload;
        raise((local)); retcode = RAISED_EVENT; RETURN_VOID();
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(PumpRequestOneEntryImpl, ()) {
        Controller::announce(announce_client_sent, 1);
        IF(randomBool("1")) 
        THEN() {
            send(servers.get(0), update, PTuple<Int, Int>(0, 1));
        }
        ELSE() {
            send(servers.get(servers.size() - 1), update, PTuple<Int, Int>(0, 1));
        }
        ENDIF()
        raise((response)); retcode = RAISED_EVENT; RETURN_VOID();
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(PumpRequestTwoEntryImpl, ()) {
        Controller::announce(announce_client_sent, 2);
        IF(randomBool("2")) 
        THEN() {
            send(servers.get(0), update, PTuple<Int, Int>(0, 2));
        }
        ELSE() {
            send(servers.get(servers.size() - 1), update, PTuple<Int, Int>(0, 2));
        }
        ENDIF()
        raise((response)); retcode = RAISED_EVENT; RETURN_VOID();
    }
    END_VOID_FUNCTION()
    /* end Function Implementations */

    /* region Machine Fields */
    PList<Ptr<PMachine>> servers;
    /* end Machine Fields  */

    /* region Jump Tables */
    inline Bool isDefered(const Int& state, const Int& event) const override {
        static const bool _isDefered[5][25] = 
            {
                { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true} /* halt */,
                { true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* Init */,
                { true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* PumpRequestOne */,
                { true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* PumpRequestTwo */,
                { true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* Done */
            };
        return getIndex2D(_isDefered, state, event);
    }

    inline Bool isGotoTransition(const Int& state, const Int& event) const override {
        static const bool _isGotoTransition[5][25] =
            {
                {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* halt */,
                {false, true,false,false,false,false,false, true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* Init */,
                {false, true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false, true} /* PumpRequestOne */,
                {false, true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false, true} /* PumpRequestTwo */,
                {false, true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* Done */
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
        static TransitionFunction _transitions[5][25] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(emptyTransition)},
                {NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(emptyTransition)},
                {NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}
            };
        #undef E
        return getIndex2D(_transitions, state, event);
    }

    inline EntryFunctionPtr getTransitionEntry(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineClient::f)
        static TransitionFunction _entries[5][25] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,NULL,NULL,NULL,E(PumpRequestOneEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(PumpRequestTwoEntry)},
                {NULL,E(haltEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(DoneEntry)},
                {NULL,E(haltEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}
            };
        #undef E
        return getIndex2D(_entries, state, event);
    }
    /* end Jump Tables */
};

};