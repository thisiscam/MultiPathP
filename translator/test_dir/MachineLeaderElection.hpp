namespace pingpong {

class MachineLeaderElection final : public PMachine {
    friend class Scheduler;
public:
    MachineLeaderElection(ExecutionEngine& engine):PMachine(engine) { }

    inline void start(const PAny& payload) override {
        InitEntry(this, payload);
    }

private:
    enum {
        Init = 1,
        SendLeader = 2
    };

    /* region Entry Methods */
    static inline void InitEntry(MachineLeaderElection* self, const PAny& payload) {
        self->states.setTop(Init);
        self->InitEntryImpl(payload);
    }

    static inline void SendLeaderEntry(MachineLeaderElection* self, const PAny& payload) {
        self->states.setTop(SendLeader);
        self->SendLeaderEntryImpl();
    }
    /* end Entry Methods */

    /* region Transition Methods */
    /* end Transition Methods */

    /* region Function Implementations */
    inline VOID_FUNCTION_DECL(InitEntryImpl, (PTuple<PList<Ptr<PMachine>>, Ptr<PMachine>, Int> payload)) {
        servers = payload.v0();
        parentServer = payload.v1();
        myRank = payload.v2();
        currentLeader = PTuple<Int, Ptr<PMachine>>(myRank, self());
        raise((local)); retcode = RAISED_EVENT; RETURN_VOID();
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(SendLeaderEntryImpl, ()) {
        currentLeader = GetNewLeader();
        passert((currentLeader.v0() <= myRank), "");
        send(parentServer, newLeader, currentLeader);
    }
    END_VOID_FUNCTION()

    inline FUNCTION_DECL(ESC(PTuple<Int, Ptr<PMachine>>), GetNewLeader, ()) {
        RETURN(PTuple<Int, Ptr<PMachine>>(1, servers.get(0)));
    }
    END_FUNCTION()
    /* end Function Implementations */

    /* region Machine Fields */
    PList<Ptr<PMachine>> servers;
    Ptr<PMachine> parentServer;
    PTuple<Int, Ptr<PMachine>> currentLeader;
    Int myRank;
    Int iter;
    /* end Machine Fields  */

    /* region Jump Tables */
    inline Bool isDefered(const Int& state, const Int& event) const override {
        static const bool _isDefered[3][25] = 
            {
                { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true} /* halt */,
                { true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* Init */,
                { true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* SendLeader */
            };
        return getIndex2D(_isDefered, state, event);
    }

    inline Bool isGotoTransition(const Int& state, const Int& event) const override {
        static const bool _isGotoTransition[3][25] =
            {
                {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* halt */,
                {false, true,false,false,false,false,false, true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* Init */,
                {false, true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* SendLeader */
            };
        return getIndex2D(_isGotoTransition, state, event);
    }

    inline ExitFunctionPtr getExitFunction(const Int& state) const override {
        #define E(f) ((ExitFunction)&MachineLeaderElection::f)
        static ExitFunction _exitFunctions[] = {&MachineLeaderElection::emptyExit,&MachineLeaderElection::emptyExit,&MachineLeaderElection::emptyExit};
        #undef E
        return getIndex1D(_exitFunctions, state);
    }

    inline TransitionFunctionPtr getTransition(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineLeaderElection::f)
        static TransitionFunction _transitions[3][25] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}
            };
        #undef E
        return getIndex2D(_transitions, state, event);
    }

    inline EntryFunctionPtr getTransitionEntry(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineLeaderElection::f)
        static TransitionFunction _entries[3][25] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,NULL,NULL,NULL,E(SendLeaderEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}
            };
        #undef E
        return getIndex2D(_entries, state, event);
    }
    /* end Jump Tables */
};

};