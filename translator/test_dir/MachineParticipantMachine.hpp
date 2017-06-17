namespace pingpong {

class MachineParticipantMachine final : public PMachine {
    friend class Scheduler;
public:
    MachineParticipantMachine(ExecutionEngine& engine):PMachine(engine) { }

    inline void start(const PAny& payload) override {
        InitEntry(payload);
    }

private:
    enum {
        Init = 1,
        WaitForRequest = 2
    };

    /* region Entry Methods */
    inline void InitEntry(const PAny& payload) {
        states.setTop(Init);
        InitEntryImpl(static_cast<PMachine*>(payload));
    }

    inline void WaitForRequestEntry(const PAny& payload) {
        states.setTop(WaitForRequest);
    }
    /* end Entry Methods */

    /* region Transition Methods */
    inline void HandlerImpl0_Wrap(const PAny& payload) {
        HandlerImpl0();
    }
    /* end Transition Methods */

    /* region Function Implementations */
    inline void InitEntryImpl(PMachine* payload) {
        auto&& tmp58 = payload;
        coor = tmp58;
        auto&& tmp59 = eUnit;
        raise(tmp59); retcode = RAISED_EVENT; return;
    }

    inline void HandlerImpl0() {
        auto&& tmp60 = randomBool();
        if(tmp60) {
            auto&& tmp61 = coor;
            auto&& tmp62 = eSuccess;
            send(tmp61, tmp62);
        } else {
            auto&& tmp63 = coor;
            auto&& tmp64 = eFailure;
            send(tmp63, tmp64);
        }
    }
    /* end Function Implementations */

    /* region Machine Fields */
    PMachine* coor;
    /* end Machine Fields  */

    /* region Jump Tables */
    inline bool isDefered(int state, int event) const override {
        static const bool _isDefered[3][8] = 
            {
                { true, true, true, true, true, true, true, true} /* halt */,
                { true,false,false,false,false,false,false,false} /* Init */,
                { true,false,false,false,false,false,false,false} /* WaitForRequest */
            };
        return _isDefered[state][event];
    }

    inline bool isGotoTransition(int state, int event) const override {
        static const bool _isGotoTransition[3][8] =
            {
                {false,false,false,false,false,false,false,false} /* halt */,
                {false, true, true,false,false,false,false,false} /* Init */,
                {false, true,false,false,false,false,false,false} /* WaitForRequest */
            };
        return _isGotoTransition[state][event];
    }

    inline ExitFunction getExitFunction(int state) const override {
        #define E(f) ((ExitFunction)&MachineParticipantMachine::f)
        static ExitFunction _exitFunctions[] = {&MachineParticipantMachine::emptyExit,&MachineParticipantMachine::emptyExit,&MachineParticipantMachine::emptyExit};
        #undef E
        return _exitFunctions[state];
    }

    inline TransitionFunction getTransition(int state, int event) const override {
        #define E(f) ((TransitionFunction)&MachineParticipantMachine::f)
        static TransitionFunction _transitions[3][8] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),E(emptyTransition),NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,E(HandlerImpl0),E(HandlerImpl0),NULL,NULL}
            };
        #undef E
        return _transitions[state][event];
    }

    inline EntryFunction getTransitionEntry(int state, int event) const override {
        #define E(f) ((TransitionFunction)&MachineParticipantMachine::f)
        static TransitionFunction _entries[3][8] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),E(WaitForRequestEntry),NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,E(emptyEntry),E(emptyEntry),NULL,NULL}
            };
        #undef E
        return _entries[state][event];
    }
    /* end Jump Tables */
};

};