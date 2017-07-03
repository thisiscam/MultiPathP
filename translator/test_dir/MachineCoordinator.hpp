namespace pingpong {

class MachineCoordinator final : public PMachine {
    friend class Scheduler;
public:
    MachineCoordinator(ExecutionEngine& engine):PMachine(engine) { }

    inline void start(const PAny& payload) override {
        InitEntry(this, payload);
    }

private:
    enum {
        Init = 1,
        Loop = 2,
        CountVote = 3,
        WaitForCancelTimerResponse = 4,
        WaitForTimeout = 5
    };

    /* region Entry Methods */
    static inline void InitEntry(MachineCoordinator* self, const PAny& payload) {
        self->states.setTop(Init);
        self->InitEntryImpl(payload);
    }

    static inline void LoopEntry(MachineCoordinator* self, const PAny& payload) {
        self->states.setTop(Loop);
    }

    static inline void CountVoteEntry(MachineCoordinator* self, const PAny& payload) {
        self->states.setTop(CountVote);
        self->CountVoteEntryImpl();
    }

    static inline void WaitForCancelTimerResponseEntry(MachineCoordinator* self, const PAny& payload) {
        self->states.setTop(WaitForCancelTimerResponse);
    }

    static inline void WaitForTimeoutEntry(MachineCoordinator* self, const PAny& payload) {
        self->states.setTop(WaitForTimeout);
    }
    /* end Entry Methods */

    /* region Transition Methods */
    static inline void HandlerImpl6_Wrap(MachineCoordinator* self, const PAny& payload) {
        self->HandlerImpl6(payload);
    }

    static inline void AnonymousHandler_7_Wrap(MachineCoordinator* self, const PAny& payload) {
        self->AnonymousHandler_7(payload);
    }

    static inline void HandlerImpl8_Wrap(MachineCoordinator* self, const PAny& payload) {
        self->HandlerImpl8(payload);
    }

    static inline void HandlerImpl4_Wrap(MachineCoordinator* self, const PAny& payload) {
        self->HandlerImpl4(payload);
    }

    static inline void HandlerImpl5_Wrap(MachineCoordinator* self, const PAny& payload) {
        self->HandlerImpl5(payload);
    }

    static inline void AnonymousHandler_9_Wrap(MachineCoordinator* self, const PAny& payload) {
        self->AnonymousHandler_9();
    }
    /* end Transition Methods */

    /* region Function Implementations */
    inline VOID_FUNCTION_DECL(InitEntryImpl, (Int payload)) {
        numReplicas = payload;
        passert((numReplicas > 0), "");
        i = 0;
        WHILE(i < numReplicas) {
            replica = create<MachineReplica>(self());
            replicas.insert(PTuple<Int, Ptr<PMachine>>(i, replica));
            i = i + 1;
        }
        ENDWHILE()
        currSeqNum = 0;
        timer = create<MachineTimer>(self());
        raise(Unit); retcode = RAISED_EVENT; RETURN_VOID();
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(DoRead, (PTuple<Ptr<PMachine>, Int> payload)) {
        IF(data.containsKey(payload.v1())) 
        THEN() {
            Controller::announce(MONITOR_READ_SUCCESS, PTuple<Int, Int>(payload.v1(), data.get(payload.v1())));
            send(payload.v0(), READ_SUCCESS, data.get(payload.v1()));
        }
        ELSE() {
            Controller::announce(MONITOR_READ_UNAVAILABLE, payload.v1());
            send(payload.v0(), READ_UNAVAILABLE);
        }
        ENDIF()
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(DoWrite, (PTuple<Ptr<PMachine>, Int, Int> payload)) {
        pendingWriteReq = payload;
        currSeqNum = currSeqNum + 1;
        i = 0;
        WHILE(i < replicas.size()) {
            send(replicas.get(i), REQ_REPLICA, PTuple<Int, Int, Int>(currSeqNum, pendingWriteReq.v1(), pendingWriteReq.v2()));
            i = i + 1;
        }
        ENDWHILE()
        send(timer, StartTimer, 100);
        raise(Unit); retcode = RAISED_EVENT; RETURN_VOID();
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(HandlerImpl4, (PTuple<Ptr<PMachine>, Int, Int> payload)) {
        DoWrite(payload);
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(HandlerImpl5, (PTuple<Ptr<PMachine>, Int> payload)) {
        DoRead(payload);
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(DoGlobalAbort, ()) {
        i = 0;
        WHILE(i < replicas.size()) {
            send(replicas.get(i), GLOBAL_ABORT, currSeqNum);
            i = i + 1;
        }
        ENDWHILE()
        send(pendingWriteReq.v0(), WRITE_FAIL);
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(CountVoteEntryImpl, ()) {
        IF(i == 0) 
        THEN() {
            WHILE(i < replicas.size()) {
                send(replicas.get(i), GLOBAL_COMMIT, currSeqNum);
                i = i + 1;
            }
            ENDWHILE()
            data.set(pendingWriteReq.v2(), pendingWriteReq.v1());
            Controller::announce(MONITOR_WRITE, PTuple<Int, Int>(pendingWriteReq.v1(), pendingWriteReq.v2()));
            send(pendingWriteReq.v0(), WRITE_SUCCESS);
            send(timer, CancelTimer);
            raise(Unit); retcode = RAISED_EVENT; RETURN_VOID();
        }
        ENDIF()
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(HandlerImpl6, (PTuple<Ptr<PMachine>, Int> payload)) {
        DoRead(payload);
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(AnonymousHandler_7, (Int payload)) {
        IF(currSeqNum == payload) 
        THEN() {
            i = i - 1;
        }
        ENDIF()
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(HandlerImpl8, (Int payload)) {
        HandleAbort(payload);
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(AnonymousHandler_9, ()) {
        DoGlobalAbort();
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(HandleAbort, (Int payload)) {
        IF(currSeqNum == payload) 
        THEN() {
            DoGlobalAbort();
            send(timer, CancelTimer);
            raise(Unit); retcode = RAISED_EVENT; RETURN_VOID();
        }
        ENDIF()
    }
    END_VOID_FUNCTION()
    /* end Function Implementations */

    /* region Machine Fields */
    PMap<Int, Int> data;
    PList<Ptr<PMachine>> replicas;
    Int numReplicas;
    Int i;
    PTuple<Ptr<PMachine>, Int, Int> pendingWriteReq;
    Ptr<PMachine> replica;
    Int currSeqNum;
    Ptr<PMachine> timer;
    /* end Machine Fields  */

    /* region Jump Tables */
    inline Bool isDefered(const Int& state, const Int& event) const override {
        static const bool _isDefered[6][22] = 
            {
                { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true} /* halt */,
                { true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* Init */,
                { true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* Loop */,
                { true,false,false,false,false,false,false, true,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* CountVote */,
                { true,false,false,false,false,false,false, true,false,false, true,false,false,false,false,false,false,false,false,false,false,false} /* WaitForCancelTimerResponse */,
                { true,false,false,false,false,false,false, true,false,false, true,false,false,false,false,false,false,false,false,false,false,false} /* WaitForTimeout */
            };
        return getIndex2D(_isDefered, state, event);
    }

    inline Bool isGotoTransition(const Int& state, const Int& event) const override {
        static const bool _isGotoTransition[6][22] =
            {
                {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* halt */,
                {false, true,false,false,false,false,false,false,false,false,false,false,false, true,false,false,false,false,false,false,false,false} /* Init */,
                {false, true,false,false,false,false,false,false,false,false,false,false,false, true,false,false,false,false,false,false,false,false} /* Loop */,
                {false, true,false, true,false,false,false,false,false,false,false,false,false, true, true,false,false,false,false,false,false,false} /* CountVote */,
                {false, true,false,false,false,false,false,false,false,false,false,false,false,false, true,false,false, true, true,false,false,false} /* WaitForCancelTimerResponse */,
                {false, true,false,false,false,false,false,false,false,false,false,false,false,false, true,false,false,false,false,false,false,false} /* WaitForTimeout */
            };
        return getIndex2D(_isGotoTransition, state, event);
    }

    inline ExitFunctionPtr getExitFunction(const Int& state) const override {
        #define E(f) ((ExitFunction)&MachineCoordinator::f)
        static ExitFunction _exitFunctions[] = {&MachineCoordinator::emptyExit,&MachineCoordinator::emptyExit,&MachineCoordinator::emptyExit,&MachineCoordinator::emptyExit,&MachineCoordinator::emptyExit,&MachineCoordinator::emptyExit};
        #undef E
        return getIndex1D(_exitFunctions, state);
    }

    inline TransitionFunctionPtr getTransition(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineCoordinator::f)
        static TransitionFunction _transitions[6][22] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,E(emptyTransition),E(emptyTransition),NULL,NULL,E(HandlerImpl4_Wrap),NULL,NULL,E(HandlerImpl5_Wrap),NULL,NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,E(AnonymousHandler_7_Wrap),E(HandlerImpl8_Wrap),NULL,NULL,NULL,NULL,NULL,E(HandlerImpl6_Wrap),NULL,NULL,E(emptyTransition),E(AnonymousHandler_9_Wrap),NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,E(emptyTransition),E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(emptyTransition),NULL,NULL,E(emptyTransition),E(emptyTransition),NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,E(emptyTransition),E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL}
            };
        #undef E
        return getIndex2D(_transitions, state, event);
    }

    inline EntryFunctionPtr getTransitionEntry(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineCoordinator::f)
        static TransitionFunction _entries[6][22] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(LoopEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,E(emptyEntry),E(emptyEntry),NULL,NULL,E(emptyEntry),NULL,NULL,E(emptyEntry),NULL,NULL,E(CountVoteEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,E(CountVoteEntry),E(emptyEntry),NULL,NULL,NULL,NULL,NULL,E(emptyEntry),NULL,NULL,E(WaitForCancelTimerResponseEntry),E(LoopEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,E(emptyEntry),E(emptyEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(LoopEntry),NULL,NULL,E(WaitForTimeoutEntry),E(LoopEntry),NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,E(emptyEntry),E(emptyEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(LoopEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL}
            };
        #undef E
        return getIndex2D(_entries, state, event);
    }
    /* end Jump Tables */
};

};