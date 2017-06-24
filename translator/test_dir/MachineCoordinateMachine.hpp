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
    inline FUNCTION_DECL(void, InitEntryImpl, (), {
        Int index;
        Ptr<PMachine> temp;
        index = 0;
        WHILE(index < 3) {
            temp = create<MachineParticipantMachine>(self());
            participants.insert(PTuple<Int, Ptr<PMachine>>(index, temp));
            index = index + 1;
        }
        ENDWHILE()
        raise(eUnit); retcode = RAISED_EVENT; RETURN();
    })

    inline FUNCTION_DECL(Ptr<PMachine>, ChooseParticipantNonDet, (), {
        Int index;
        index = 0;
        WHILE(index < participants.size()) {
            IF(randomBool("1")) 
            THEN({
                RETURN( participants.get(index));
            })ENDIF()
            index = index + 1;
        }
        ENDWHILE()
        RETURN( participants.get(0));
    })

    inline FUNCTION_DECL(void, TransactionStateEntryImpl, (), {
        Ptr<PMachine> p;
        p = ChooseParticipantNonDet();
        IF(randomBool("2")) 
        THEN({
            send(p, eCommit);
        }) 
        ELSE({
            send(p, eAbort);
        })
        ENDIF()
    })
    /* end Function Implementations */

    /* region Machine Fields */
    PList<Ptr<PMachine>> participants;
    /* end Machine Fields  */

    /* region Jump Tables */
    inline Bool isDefered(const Int& state, const Int& event) const override {
        static const bool _isDefered[4][8] = 
            {
                { true, true, true, true, true, true, true, true} /* halt */,
                { true,false,false,false,false,false,false,false} /* Init */,
                { true,false,false,false,false,false,false,false} /* WaitForRequest */,
                { true,false,false, true,false,false,false,false} /* TransactionState */
            };
        return getIndex2D(_isDefered, state, event);
    }

    inline Bool isGotoTransition(const Int& state, const Int& event) const override {
        static const bool _isGotoTransition[4][8] =
            {
                {false,false,false,false,false,false,false,false} /* halt */,
                {false, true, true,false,false,false,false,false} /* Init */,
                {false, true,false, true,false,false,false,false} /* WaitForRequest */,
                {false, true,false,false,false,false, true, true} /* TransactionState */
            };
        return getIndex2D(_isGotoTransition, state, event);
    }

    inline ExitFunctionPtr getExitFunction(const Int& state) const override {
        #define E(f) ((ExitFunction)&MachineCoordinateMachine::f)
        static ExitFunction _exitFunctions[] = {&MachineCoordinateMachine::emptyExit,&MachineCoordinateMachine::emptyExit,&MachineCoordinateMachine::emptyExit,&MachineCoordinateMachine::emptyExit};
        #undef E
        return getIndex1D(_exitFunctions, state);
    }

    inline TransitionFunctionPtr getTransition(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineCoordinateMachine::f)
        static TransitionFunction _transitions[4][8] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),E(emptyTransition),NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,E(emptyTransition),NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,NULL,NULL,E(emptyTransition),E(emptyTransition)}
            };
        #undef E
        return getIndex2D(_transitions, state, event);
    }

    inline EntryFunctionPtr getTransitionEntry(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineCoordinateMachine::f)
        static TransitionFunction _entries[4][8] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),E(WaitForRequestEntry),NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,E(TransactionStateEntry),NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,NULL,NULL,E(WaitForRequestEntry),E(WaitForRequestEntry)}
            };
        #undef E
        return getIndex2D(_entries, state, event);
    }
    /* end Jump Tables */
};

};