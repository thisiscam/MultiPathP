namespace pingpong {

class MachineReplica final : public PMachine {
    friend class Scheduler;
public:
    MachineReplica(ExecutionEngine& engine):PMachine(engine) { }

    inline void start(const PAny& payload) override {
        InitEntry(this, payload);
    }

private:
    enum {
        Init = 1,
        Loop = 2
    };

    /* region Entry Methods */
    static inline void InitEntry(MachineReplica* self, const PAny& payload) {
        self->states.setTop(Init);
        self->InitEntryImpl(payload);
    }

    static inline void LoopEntry(MachineReplica* self, const PAny& payload) {
        self->states.setTop(Loop);
    }
    /* end Entry Methods */

    /* region Transition Methods */
    static inline void HandlerImpl2_Wrap(MachineReplica* self, const PAny& payload) {
        self->HandlerImpl2(payload);
    }

    static inline void HandlerImpl3_Wrap(MachineReplica* self, const PAny& payload) {
        self->HandlerImpl3(payload);
    }

    static inline void HandlerImpl1_Wrap(MachineReplica* self, const PAny& payload) {
        self->HandlerImpl1(payload);
    }
    /* end Transition Methods */

    /* region Function Implementations */
    inline VOID_FUNCTION_DECL(InitEntryImpl, (Ptr<PMachine> payload)) {
        coordinator = payload;
        lastSeqNum = 0;
        raise(Unit); retcode = RAISED_EVENT; RETURN_VOID();
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(HandleReqReplica, (PTuple<Int, Int, Int> payload)) {
        pendingWriteReq = payload;
        passert((pendingWriteReq.v0() > lastSeqNum), "");
        shouldCommit = ShouldCommitWrite();
        IF(shouldCommit) 
        THEN() {
            send(coordinator, RESP_REPLICA_COMMIT, pendingWriteReq.v0());
        }
        ELSE() {
            send(coordinator, RESP_REPLICA_ABORT, pendingWriteReq.v0());
        }
        ENDIF()
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(HandlerImpl1, (Int payload)) {
        passert((pendingWriteReq.v0() >= payload), "");
        IF(pendingWriteReq.v0() == payload) 
        THEN() {
            lastSeqNum = payload;
        }
        ENDIF()
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(HandlerImpl2, (Int payload)) {
        passert((pendingWriteReq.v0() >= payload), "");
        IF(pendingWriteReq.v0() == payload) 
        THEN() {
            data.set(pendingWriteReq.v2(), pendingWriteReq.v1());
            lastSeqNum = payload;
        }
        ENDIF()
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(HandlerImpl3, (PTuple<Int, Int, Int> payload)) {
        HandleReqReplica(payload);
    }
    END_VOID_FUNCTION()

    inline FUNCTION_DECL(ESC(Bool), ShouldCommitWrite, ()) {
        RETURN(randomBool("2"));
    }
    END_FUNCTION()
    /* end Function Implementations */

    /* region Machine Fields */
    Ptr<PMachine> coordinator;
    PMap<Int, Int> data;
    PTuple<Int, Int, Int> pendingWriteReq;
    Bool shouldCommit;
    Int lastSeqNum;
    /* end Machine Fields  */

    /* region Jump Tables */
    inline Bool isDefered(const Int& state, const Int& event) const override {
        static const bool _isDefered[3][22] = 
            {
                { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true} /* halt */,
                { true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* Init */,
                { true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* Loop */
            };
        return getIndex2D(_isDefered, state, event);
    }

    inline Bool isGotoTransition(const Int& state, const Int& event) const override {
        static const bool _isGotoTransition[3][22] =
            {
                {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* halt */,
                {false, true,false,false,false,false,false,false,false,false,false,false,false, true,false,false,false,false,false,false,false,false} /* Init */,
                {false, true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* Loop */
            };
        return getIndex2D(_isGotoTransition, state, event);
    }

    inline ExitFunctionPtr getExitFunction(const Int& state) const override {
        #define E(f) ((ExitFunction)&MachineReplica::f)
        static ExitFunction _exitFunctions[] = {&MachineReplica::emptyExit,&MachineReplica::emptyExit,&MachineReplica::emptyExit};
        #undef E
        return getIndex1D(_exitFunctions, state);
    }

    inline TransitionFunctionPtr getTransition(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineReplica::f)
        static TransitionFunction _transitions[3][22] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),E(HandlerImpl3_Wrap),NULL,NULL,E(HandlerImpl1_Wrap),E(HandlerImpl2_Wrap),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}
            };
        #undef E
        return getIndex2D(_transitions, state, event);
    }

    inline EntryFunctionPtr getTransitionEntry(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineReplica::f)
        static TransitionFunction _entries[3][22] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(LoopEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),E(emptyEntry),NULL,NULL,E(emptyEntry),E(emptyEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}
            };
        #undef E
        return getIndex2D(_entries, state, event);
    }
    /* end Jump Tables */
};

};