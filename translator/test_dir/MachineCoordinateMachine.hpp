namespace pingpong {

class MachineCoordinateMachine final : public PMachine {
    friend class Scheduler;
public:
    MachineCoordinateMachine(ExecutionEngine& engine):PMachine(engine) { }

    inline void start(const PAny& payload) override {
        InitEntry(payload);
    }

private:
    enum {
        Init = 1,
        WaitForRequest = 2,
        TransactionState = 3
    };

    /* region Entry Methods */
    inline void InitEntry(const PAny& payload) {
        states.setTop(Init);
        InitEntryImpl();
    }

    inline void WaitForRequestEntry(const PAny& payload) {
        states.setTop(WaitForRequest);
    }

    inline void TransactionStateEntry(const PAny& payload) {
        states.setTop(TransactionState);
        TransactionStateEntryImpl();
    }
    /* end Entry Methods */

    /* region Transition Methods */
    /* end Transition Methods */

    /* region Function Implementations */
    inline void InitEntryImpl() {
        int index;
        PMachine* temp;
        auto&& tmp17 = 0;
        index = tmp17;
        auto&& tmp18 = index;
        auto&& tmp19 = 3;
        auto&& tmp20 = tmp18 < tmp19;
        while(tmp20) {
            auto&& tmp22 = this;
            auto&& tmp23 = create<MachineParticipantMachine>(tmp22);
            temp = tmp23;
            auto&& tmp24 = participants;
            auto&& tmp25 = index;
            auto&& tmp26 = temp;
            auto&& tmp27 = PTuple<int, PMachine*>(tmp25, tmp26);
            tmp24.insert(tmp27);
            auto&& tmp29 = index;
            auto&& tmp30 = 1;
            auto&& tmp31 = tmp29 + tmp30;
            index = tmp31;
            auto&& tmp18 = index;
            auto&& tmp19 = 3;
            tmp20 = tmp18 < tmp19;
        }
        auto&& tmp32 = eUnit;
        raise(tmp32); retcode = RAISED_EVENT; return;
    }

    inline PMachine* ChooseParticipantNonDet() {
        int index;
        auto&& tmp34 = 0;
        index = tmp34;
        auto&& tmp35 = index;
        auto&& tmp36 = participants;
        auto&& tmp37 = tmp36.size();
        auto&& tmp38 = tmp35 < tmp37;
        while(tmp38) {
            auto&& tmp39 = randomBool();
            if(tmp39) {
                auto&& tmp40 = participants;
                auto&& tmp41 = index;
                auto&& tmp42 = tmp40.get(tmp41);
                return tmp42;
            }
            auto&& tmp44 = index;
            auto&& tmp45 = 1;
            auto&& tmp46 = tmp44 + tmp45;
            index = tmp46;
            auto&& tmp35 = index;
            auto&& tmp36 = participants;
            auto&& tmp37 = tmp36.size();
            tmp38 = tmp35 < tmp37;
        }
        auto&& tmp47 = participants;
        auto&& tmp48 = 0;
        auto&& tmp49 = tmp47.get(tmp48);
        return tmp49;
    }

    inline void TransactionStateEntryImpl() {
        PMachine* p;

        auto&& tmp51 = ChooseParticipantNonDet();

        p = tmp51;
        auto&& tmp52 = randomBool();
        if(tmp52) {
            auto&& tmp53 = p;
            auto&& tmp54 = eCommit;
            send(tmp53, tmp54);
        } else {
            auto&& tmp55 = p;
            auto&& tmp56 = eAbort;
            send(tmp55, tmp56);
        }
    }
    /* end Function Implementations */

    /* region Machine Fields */
    PList<PMachine*> participants;
    /* end Machine Fields  */

    /* region Jump Tables */
    inline bool isDefered(int state, int event) const override {
        static const bool _isDefered[4][8] = 
            {
                { true, true, true, true, true, true, true, true} /* halt */,
                { true,false,false,false,false,false,false,false} /* Init */,
                { true,false,false,false,false,false,false,false} /* WaitForRequest */,
                { true,false,false, true,false,false,false,false} /* TransactionState */
            };
        return _isDefered[state][event];
    }

    inline bool isGotoTransition(int state, int event) const override {
        static const bool _isGotoTransition[4][8] =
            {
                {false,false,false,false,false,false,false,false} /* halt */,
                {false, true, true,false,false,false,false,false} /* Init */,
                {false, true,false, true,false,false,false,false} /* WaitForRequest */,
                {false, true,false,false,false,false, true, true} /* TransactionState */
            };
        return _isGotoTransition[state][event];
    }

    inline ExitFunction getExitFunction(int state) const override {
        #define E(f) ((ExitFunction)&MachineCoordinateMachine::f)
        static ExitFunction _exitFunctions[] = {&MachineCoordinateMachine::emptyExit,&MachineCoordinateMachine::emptyExit,&MachineCoordinateMachine::emptyExit,&MachineCoordinateMachine::emptyExit};
        #undef E
        return _exitFunctions[state];
    }

    inline TransitionFunction getTransition(int state, int event) const override {
        #define E(f) ((TransitionFunction)&MachineCoordinateMachine::f)
        static TransitionFunction _transitions[4][8] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),E(emptyTransition),NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,E(emptyTransition),NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,NULL,NULL,E(emptyTransition),E(emptyTransition)}
            };
        #undef E
        return _transitions[state][event];
    }

    inline EntryFunction getTransitionEntry(int state, int event) const override {
        #define E(f) ((TransitionFunction)&MachineCoordinateMachine::f)
        static TransitionFunction _entries[4][8] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),E(WaitForRequestEntry),NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,E(TransactionStateEntry),NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,NULL,NULL,E(WaitForRequestEntry),E(WaitForRequestEntry)}
            };
        #undef E
        return _entries[state][event];
    }
    /* end Jump Tables */
};

};