namespace pingpong {

class MachinePaxosNode final : public PMachine {
    friend class Scheduler;
public:
    MachinePaxosNode(ExecutionEngine& engine):PMachine(engine) { }

    inline void start(const PAny& payload) override {
        InitEntry(this, payload);
    }

private:
    enum {
        Init = 1,
        PerformOperation = 2,
        ProposeValuePhase1 = 3,
        ProposeValuePhase2 = 4,
        RunLearner = 5
    };

    /* region Entry Methods */
    static inline void InitEntry(MachinePaxosNode* self, const PAny& payload) {
        self->states.setTop(Init);
        self->InitEntryImpl(payload);
    }

    static inline void PerformOperationEntry(MachinePaxosNode* self, const PAny& payload) {
        self->states.setTop(PerformOperation);
    }

    static inline void ProposeValuePhase1Entry(MachinePaxosNode* self, const PAny& payload) {
        self->states.setTop(ProposeValuePhase1);
        self->ProposeValuePhase1EntryImpl(payload);
    }

    static inline void ProposeValuePhase2Entry(MachinePaxosNode* self, const PAny& payload) {
        self->states.setTop(ProposeValuePhase2);
        self->ProposeValuePhase2EntryImpl();
    }

    static inline void RunLearnerEntry(MachinePaxosNode* self, const PAny& payload) {
        self->states.setTop(RunLearner);
        self->RunLearnerEntryImpl(payload);
    }
    /* end Entry Methods */

    /* region Transition Methods */
    static inline void HandlerImpl4_Wrap(MachinePaxosNode* self, const PAny& payload) {
        self->HandlerImpl4(payload);
    }

    static inline void HandlerImpl0_Wrap(MachinePaxosNode* self, const PAny& payload) {
        self->HandlerImpl0(payload);
    }

    static inline void HandlerImpl9_Wrap(MachinePaxosNode* self, const PAny& payload) {
        self->HandlerImpl9(payload);
    }

    static inline void AnonymousHandler_7_Wrap(MachinePaxosNode* self, const PAny& payload) {
        self->AnonymousHandler_7(payload);
    }

    static inline void HandlerImpl3_Wrap(MachinePaxosNode* self, const PAny& payload) {
        self->HandlerImpl3(payload);
    }

    static inline void AnonymousHandler_10_Wrap(MachinePaxosNode* self, const PAny& payload) {
        self->AnonymousHandler_10(payload);
    }

    static inline void HandlerImpl5_Wrap(MachinePaxosNode* self, const PAny& payload) {
        self->HandlerImpl5(payload);
    }

    static inline void HandlerImpl6_Wrap(MachinePaxosNode* self, const PAny& payload) {
        self->HandlerImpl6(payload);
    }

    static inline void HandlerImpl1_Wrap(MachinePaxosNode* self, const PAny& payload) {
        self->HandlerImpl1(payload);
    }

    static inline void HandlerImpl2_Wrap(MachinePaxosNode* self, const PAny& payload) {
        self->HandlerImpl2(payload);
    }

    static inline void AnonymousHandler_8_Wrap(MachinePaxosNode* self, const PAny& payload) {
        self->AnonymousHandler_8();
    }
    /* end Transition Methods */

    /* region Function Implementations */
    inline VOID_FUNCTION_DECL(InitEntryImpl, (PTuple<Int> payload)) {
        myRank = payload.v0();
        currentLeader = PTuple<Int, Ptr<PMachine>>(myRank, self());
        roundNum = 0;
        maxRound = 0;
        timer = create<MachineTimer>(PTuple<Ptr<PMachine>, Int>(self(), 10));
        lastExecutedSlot = -1;
        nextSlotForProposer = 0;
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(HandlerImpl0, (PTuple<PList<Ptr<PMachine>>> payload)) {
        UpdateAcceptors(payload);
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(UpdateAcceptors, (PTuple<PList<Ptr<PMachine>>> payload)) {
        acceptors = payload.v0();
        majority = (acceptors.size()) / 2 + 1;
        passert((majority == 2), "");
        leaderElectionService = create<MachineLeaderElection>(PTuple<PList<Ptr<PMachine>>, Ptr<PMachine>, Int>(acceptors, self(), myRank));
        raise((local)); retcode = RAISED_EVENT; RETURN_VOID();
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(CheckIfLeader, (PTuple<Int, Int> payload)) {
        IF(currentLeader.v0() == myRank) 
        THEN() {
            commitValue = payload.v1();
            proposeVal = commitValue;
            raise((goPropose)); retcode = RAISED_EVENT; RETURN_VOID();
        }
        ELSE() {
            send(currentLeader.v1(), update, payload);
        }
        ENDIF()
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(HandlerImpl1, (PTuple<Int, Int> payload)) {
        CheckIfLeader(payload);
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(HandlerImpl2, (PTuple<Ptr<PMachine>, Int, PTuple<Int, Int>> payload)) {
        preparefun(payload);
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(HandlerImpl3, (PTuple<Ptr<PMachine>, Int, PTuple<Int, Int>, Int> payload)) {
        acceptfun(payload);
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(HandlerImpl4, (PTuple<Int, Ptr<PMachine>> payload)) {
        send(leaderElectionService, Ping, payload);
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(HandlerImpl5, (PTuple<Int, Ptr<PMachine>> payload)) {
        currentLeader = payload;
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(preparefun, (PTuple<Ptr<PMachine>, Int, PTuple<Int, Int>> receivedMess_2)) {
        IF(!(acceptorSlots.containsKey(receivedMess_2.v1()))) 
        THEN() {
            send(receivedMess_2.v0(), agree, PTuple<Int, PTuple<Int, Int>, Int, Int>(receivedMess_2.v1(), PTuple<Int, Int>(-1, -1), -1, -1));
            acceptorSlots.set(PTuple<PTuple<Int, Int>, Int>(receivedMess_2.v2(), -1), receivedMess_2.v1());
        }
        ENDIF()
        returnVal = lessThan(receivedMess_2.v2(),acceptorSlots.get(receivedMess_2.v1()).v0());
        IF(returnVal) 
        THEN() {
            send(receivedMess_2.v0(), reject, PTuple<Int, PTuple<Int, Int>>(receivedMess_2.v1(), acceptorSlots.get(receivedMess_2.v1()).v0()));
        }
        ELSE() {
            send(receivedMess_2.v0(), agree, PTuple<Int, PTuple<Int, Int>, Int>(receivedMess_2.v1(), acceptorSlots.get(receivedMess_2.v1()).v0(), acceptorSlots.get(receivedMess_2.v1()).v1()));
            acceptorSlots.set(PTuple<PTuple<Int, Int>, Int>(receivedMess_2.v2(), -1), receivedMess_2.v1());
        }
        ENDIF()
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(acceptfun, (PTuple<Ptr<PMachine>, Int, PTuple<Int, Int>, Int> receivedMess_2)) {
        IF(acceptorSlots.containsKey(receivedMess_2.v1())) 
        THEN() {
            returnVal = equal(receivedMess_2.v2(),acceptorSlots.get(receivedMess_2.v1()).v0());
            IF(!returnVal) 
            THEN() {
                send(receivedMess_2.v0(), reject, PTuple<Int, PTuple<Int, Int>>(receivedMess_2.v1(), acceptorSlots.get(receivedMess_2.v1()).v0()));
            }
            ELSE() {
                acceptorSlots.set(PTuple<PTuple<Int, Int>, Int>(receivedMess_2.v2(), receivedMess_2.v3()), receivedMess_2.v1());
                send(receivedMess_2.v0(), accepted, PTuple<Int, PTuple<Int, Int>, Int>(receivedMess_2.v1(), receivedMess_2.v2(), receivedMess_2.v3()));
            }
            ENDIF()
        }
        ENDIF()
    }
    END_VOID_FUNCTION()

    inline FUNCTION_DECL(ESC(PTuple<Int, Int>), GetNextProposal, (Int maxRound)) {
        RETURN(PTuple<Int, Int>(maxRound + 1, myRank));
    }
    END_FUNCTION()

    inline FUNCTION_DECL(ESC(Bool), equal, (PTuple<Int, Int> p1, PTuple<Int, Int> p2)) {
        IF(p1.v0() == p2.v0() & p1.v1() == p2.v1()) 
        THEN() {
            RETURN(true);
        }
        ELSE() {
            RETURN(false);
        }
        ENDIF()
    }
    END_FUNCTION()

    inline FUNCTION_DECL(ESC(Bool), lessThan, (PTuple<Int, Int> p1, PTuple<Int, Int> p2)) {
        IF(p1.v0() < p2.v0()) 
        THEN() {
            RETURN(true);
        }
        ELSE() {
            IF(p1.v0() == p2.v0()) 
            THEN() {
                IF(p1.v1() < p2.v1()) 
                THEN() {
                    RETURN(true);
                }
                ELSE() {
                    RETURN(false);
                }
                ENDIF()
            }
            ELSE() {
                RETURN(false);
            }
            ENDIF()
        }
        ENDIF()
    }
    END_FUNCTION()

    inline VOID_FUNCTION_DECL(BroadCastAcceptors, (Int mess, PAny pay)) {
        iter = 0;
        WHILE(iter < acceptors.size()) {
            send(acceptors.get(iter), mess, pay);
            iter = iter + 1;
        }
        ENDWHILE()
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(ProposeValuePhase1EntryImpl, (PAny payload)) {
        countAgree = 0;
        nextProposal = GetNextProposal(maxRound);
        receivedAgree = PTuple<PTuple<Int, Int>, Int>(PTuple<Int, Int>(-1, -1), -1);
        BroadCastAcceptors(prepare,PTuple<Ptr<PMachine>, Int, PTuple<Int, Int>, Int>(self(), nextSlotForProposer, PTuple<Int, Int>(nextProposal.v0(), myRank), myRank));
        Controller::announce(announce_proposer_sent, proposeVal);
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(HandlerImpl6, (PTuple<Int, PTuple<Int, Int>, Int> receivedMess)) {
        IF(receivedMess.v0() == nextSlotForProposer) 
        THEN() {
            countAgree = countAgree + 1;
            returnVal = lessThan(receivedAgree.v0(),receivedMess.v1());
            IF(returnVal) 
            THEN() {
                receivedAgree.set(receivedMess.v1(), PTupleIndexer::V0());
                receivedAgree.set(receivedMess.v2(), PTupleIndexer::V1());
            }
            ENDIF()
            IF(countAgree == majority) 
            THEN() {
                raise((success)); retcode = RAISED_EVENT; RETURN_VOID();
            }
            ENDIF()
        }
        ENDIF()
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(AnonymousHandler_7, (PTuple<Int, PTuple<Int, Int>> payload)) {
        IF(nextProposal.v0() <= payload.v1().v0()) 
        THEN() {
            maxRound = payload.v1().v0();
        }
        ENDIF()
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(AnonymousHandler_8, ()) {

    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(CountAccepted, (PTuple<Int, PTuple<Int, Int>, Int> receivedMess_1)) {
        IF(receivedMess_1.v0() == nextSlotForProposer) 
        THEN() {
            returnVal = equal(receivedMess_1.v1(),nextProposal);
            IF(returnVal) 
            THEN() {
                countAccept = countAccept + 1;
            }
            ENDIF()
            IF(countAccept == majority) 
            THEN() {
                nextSlotForProposer = nextSlotForProposer + 1;
                raise(chosen, receivedMess_1); retcode = RAISED_EVENT; RETURN_VOID();
            }
            ENDIF()
        }
        ENDIF()
    }
    END_VOID_FUNCTION()

    inline FUNCTION_DECL(ESC(Int), getHighestProposedValue, ()) {
        IF(receivedAgree.v1() != -1) 
        THEN() {
            currCommitOperation = false;
            RETURN(receivedAgree.v1());
        }
        ELSE() {
            currCommitOperation = true;
            RETURN(commitValue);
        }
        ENDIF()
    }
    END_FUNCTION()

    inline VOID_FUNCTION_DECL(ProposeValuePhase2EntryImpl, ()) {
        countAccept = 0;
        proposeVal = getHighestProposedValue();
        Controller::announce(announce_valueProposed, PTuple<Ptr<PMachine>, Int, PTuple<Int, Int>, Int>(self(), nextSlotForProposer, nextProposal, proposeVal));
        Controller::announce(announce_proposer_sent, proposeVal);
        BroadCastAcceptors(accept,PTuple<Ptr<PMachine>, Int, PTuple<Int, Int>, Int>(self(), nextSlotForProposer, nextProposal, proposeVal));
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(HandlerImpl9, (PTuple<Int, PTuple<Int, Int>, Int> payload)) {
        CountAccepted(payload);
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(AnonymousHandler_10, (PTuple<Int, PTuple<Int, Int>> payload)) {
        IF(nextProposal.v0() <= payload.v1().v0()) 
        THEN() {
            maxRound = payload.v1().v0();
        }
        ENDIF()
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(RunReplicatedMachine, ()) {
        WHILE(true) {
            IF(learnerSlots.containsKey((lastExecutedSlot + 1))) 
            THEN() {
                lastExecutedSlot = lastExecutedSlot + 1;
            }
            ELSE() {
                RETURN_VOID();
            }
            ENDIF()
        }
        ENDWHILE()
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(RunLearnerEntryImpl, (PTuple<Int, PTuple<Int, Int>, Int> receivedMess_1)) {
        learnerSlots.set(PTuple<PTuple<Int, Int>, Int>(receivedMess_1.v1(), receivedMess_1.v2()), receivedMess_1.v0());
        RunReplicatedMachine();
        IF(currCommitOperation & commitValue == receivedMess_1.v2()) 
        THEN() {
            popState();
            retcode = RAISED_EVENT; RETURN_VOID();
        }
        ELSE() {
            proposeVal = commitValue;
            raise((goPropose)); retcode = RAISED_EVENT; RETURN_VOID();
        }
        ENDIF()
    }
    END_VOID_FUNCTION()
    /* end Function Implementations */

    /* region Machine Fields */
    PTuple<Int, Ptr<PMachine>> currentLeader;
    Ptr<PMachine> leaderElectionService;
    PList<Ptr<PMachine>> acceptors;
    Int commitValue;
    Int proposeVal;
    Int majority;
    Int roundNum;
    Int myRank;
    PTuple<Int, Int> nextProposal;
    PTuple<PTuple<Int, Int>, Int> receivedAgree;
    Int iter;
    Int maxRound;
    Int countAccept;
    Int countAgree;
    Int tempVal;
    Bool returnVal;
    Ptr<PMachine> timer;
    PTuple<Int, PTuple<Int, Int>, Int> receivedMess_1;
    Int nextSlotForProposer;
    Bool currCommitOperation;
    PMap<Int, PTuple<PTuple<Int, Int>, Int>> acceptorSlots;
    PTuple<Ptr<PMachine>, Int, PTuple<Int, Int>, Int> receivedMess_2;
    PMap<Int, PTuple<PTuple<Int, Int>, Int>> learnerSlots;
    Int lastExecutedSlot;
    /* end Machine Fields  */

    /* region Jump Tables */
    inline Bool isDefered(const Int& state, const Int& event) const override {
        static const bool _isDefered[6][25] = 
            {
                { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true} /* halt */,
                { true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false, true,false,false,false,false,false,false} /* Init */,
                { true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* PerformOperation */,
                { true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* ProposeValuePhase1 */,
                { true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* ProposeValuePhase2 */,
                { true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false, true,false,false,false,false,false} /* RunLearner */
            };
        return getIndex2D(_isDefered, state, event);
    }

    inline Bool isGotoTransition(const Int& state, const Int& event) const override {
        static const bool _isGotoTransition[6][25] =
            {
                {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* halt */,
                {false, true,false,false,false,false,false, true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* Init */,
                {false, true,false,false,false,false,false,false,false,false, true, true,false,false,false,false,false,false,false,false,false,false,false,false,false} /* PerformOperation */,
                {false, true,false,false,false, true,false,false, true,false,false,false,false,false,false,false,false,false,false,false, true,false,false,false,false} /* ProposeValuePhase1 */,
                {false, true,false,false,false, true,false,false,false,false,false,false,false,false,false,false,false,false,false,false, true,false,false,false,false} /* ProposeValuePhase2 */,
                {false, true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* RunLearner */
            };
        return getIndex2D(_isGotoTransition, state, event);
    }

    inline ExitFunctionPtr getExitFunction(const Int& state) const override {
        #define E(f) ((ExitFunction)&MachinePaxosNode::f)
        static ExitFunction _exitFunctions[] = {&MachinePaxosNode::emptyExit,&MachinePaxosNode::emptyExit,&MachinePaxosNode::emptyExit,&MachinePaxosNode::emptyExit,&MachinePaxosNode::emptyExit,&MachinePaxosNode::emptyExit};
        #undef E
        return getIndex1D(_exitFunctions, state);
    }

    inline TransitionFunctionPtr getTransition(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachinePaxosNode::f)
        static TransitionFunction _transitions[6][25] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,E(emptyTransition),NULL,E(HandlerImpl0_Wrap),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),E(HandlerImpl2_Wrap),E(HandlerImpl3_Wrap),E(emptyTransition),NULL,E(emptyTransition),NULL,NULL,NULL,E(HandlerImpl1_Wrap),E(HandlerImpl3_Wrap),E(HandlerImpl1_Wrap),NULL,NULL,NULL,NULL,NULL,E(HandlerImpl4_Wrap),E(HandlerImpl5_Wrap),E(emptyTransition),NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,E(HandlerImpl6_Wrap),E(AnonymousHandler_7_Wrap),E(emptyTransition),NULL,E(AnonymousHandler_8_Wrap),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(emptyTransition),NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,E(emptyTransition),E(AnonymousHandler_10_Wrap),E(HandlerImpl9_Wrap),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(emptyTransition),NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),E(emptyTransition),E(emptyTransition),E(emptyTransition),E(emptyTransition),E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(emptyTransition),NULL,NULL,NULL,NULL}
            };
        #undef E
        return getIndex2D(_transitions, state, event);
    }

    inline EntryFunctionPtr getTransitionEntry(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachinePaxosNode::f)
        static TransitionFunction _entries[6][25] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,NULL,NULL,NULL,E(PerformOperationEntry),NULL,E(emptyEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),E(emptyEntry),E(emptyEntry),E(emptyEntry),NULL,E(emptyEntry),NULL,NULL,NULL,E(ProposeValuePhase1Entry),E(RunLearnerEntry),E(emptyEntry),NULL,NULL,NULL,NULL,NULL,E(emptyEntry),E(emptyEntry),E(emptyEntry),NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,E(emptyEntry),E(ProposeValuePhase1Entry),E(emptyEntry),NULL,E(ProposeValuePhase2Entry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(ProposeValuePhase1Entry),NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,E(emptyEntry),E(ProposeValuePhase1Entry),E(emptyEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(ProposeValuePhase1Entry),NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),E(emptyEntry),E(emptyEntry),E(emptyEntry),E(emptyEntry),E(emptyEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(emptyEntry),NULL,NULL,NULL,NULL}
            };
        #undef E
        return getIndex2D(_entries, state, event);
    }
    /* end Jump Tables */
};

};