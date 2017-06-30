namespace pingpong {

class MachinePING final : public PMachine {
    friend class Scheduler;
public:
    MachinePING(ExecutionEngine& engine):PMachine(engine) { }

    inline void start(const PAny& payload) override {
        Ping_InitEntry(payload);
    }

private:
    enum {
        Ping_Init = 1,
        Ping_SendPing = 2,
        Ping_WaitPong = 3,
        Done = 4
    };

    /* region Entry Methods */
    inline void Ping_InitEntry(const PAny& payload) {
        states.setTop(Ping_Init);
        Ping_InitEntryImpl();
    }

    inline void Ping_SendPingEntry(const PAny& payload) {
        states.setTop(Ping_SendPing);
        Ping_SendPingEntryImpl();
    }

    inline void Ping_WaitPongEntry(const PAny& payload) {
        states.setTop(Ping_WaitPong);
    }

    inline void DoneEntry(const PAny& payload) {
        states.setTop(Done);
    }
    /* end Entry Methods */

    /* region Transition Methods */
    /* end Transition Methods */

    /* region Function Implementations */
    inline VOID_FUNCTION_DECL(Ping_InitEntryImpl, ()) {
        pongId = create<MachinePONG>();
        raise(Success); retcode = RAISED_EVENT; RETURN_VOID();
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(Ping_SendPingEntryImpl, ()) {
        send(pongId, Ping, self());
        raise(Success); retcode = RAISED_EVENT; RETURN_VOID();
    }
    END_VOID_FUNCTION()
    /* end Function Implementations */

    /* region Machine Fields */
    Ptr<PMachine> pongId;
    /* end Machine Fields  */

    /* region Jump Tables */
    inline Bool isDefered(const Int& state, const Int& event) const override {
        static const bool _isDefered[5][5] = 
            {
                { true, true, true, true, true} /* halt */,
                { true,false,false,false,false} /* Ping_Init */,
                { true,false,false,false,false} /* Ping_SendPing */,
                { true,false,false,false,false} /* Ping_WaitPong */,
                { true,false,false,false,false} /* Done */
            };
        return getIndex2D(_isDefered, state, event);
    }

    inline Bool isGotoTransition(const Int& state, const Int& event) const override {
        static const bool _isGotoTransition[5][5] =
            {
                {false,false,false,false,false} /* halt */,
                {false, true,false,false, true} /* Ping_Init */,
                {false, true,false,false, true} /* Ping_SendPing */,
                {false, true,false, true,false} /* Ping_WaitPong */,
                {false, true,false,false,false} /* Done */
            };
        return getIndex2D(_isGotoTransition, state, event);
    }

    inline ExitFunctionPtr getExitFunction(const Int& state) const override {
        #define E(f) ((ExitFunction)&MachinePING::f)
        static ExitFunction _exitFunctions[] = {&MachinePING::emptyExit,&MachinePING::emptyExit,&MachinePING::emptyExit,&MachinePING::emptyExit,&MachinePING::emptyExit};
        #undef E
        return getIndex1D(_exitFunctions, state);
    }

    inline TransitionFunctionPtr getTransition(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachinePING::f)
        static TransitionFunction _transitions[5][5] = 
            {
                {NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,E(emptyTransition)},
                {NULL,E(emptyTransition),NULL,NULL,E(emptyTransition)},
                {NULL,E(emptyTransition),NULL,E(emptyTransition),NULL},
                {NULL,E(emptyTransition),NULL,NULL,NULL}
            };
        #undef E
        return getIndex2D(_transitions, state, event);
    }

    inline EntryFunctionPtr getTransitionEntry(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachinePING::f)
        static TransitionFunction _entries[5][5] = 
            {
                {NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,E(Ping_SendPingEntry)},
                {NULL,E(haltEntry),NULL,NULL,E(Ping_WaitPongEntry)},
                {NULL,E(haltEntry),NULL,E(Ping_SendPingEntry),NULL},
                {NULL,E(haltEntry),NULL,NULL,NULL}
            };
        #undef E
        return getIndex2D(_entries, state, event);
    }
    /* end Jump Tables */
};

};