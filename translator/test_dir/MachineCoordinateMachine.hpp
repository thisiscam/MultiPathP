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
        index = 0;
        while(index < 3) {
            temp = create<MachineParticipantMachine>(this);
            participants.insert(PTuple<int, PMachine*>(index, temp));
            index = index + 1;
        }
        raise(eUnit); retcode = RAISED_EVENT; return;
    }

    inline PMachine* ChooseParticipantNonDet() {
        int index;
        index = 0;
        while(index < participants.size()) {
            if(randomBool()) {
                return participants.get(index);
            }
            index = index + 1;
        }
        return participants.get(0);
    }

    inline void TransactionStateEntryImpl() {
        PMachine* p;
        p = ChooseParticipantNonDet();
        if(randomBool()) {
            send(p, eCommit);
        } else {
            send(p, eAbort);
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