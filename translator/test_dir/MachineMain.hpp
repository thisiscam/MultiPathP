namespace pingpong {

class MachineMain final : public PMachine {
    friend class Scheduler;
public:
    MachineMain(ExecutionEngine& engine):PMachine(engine) { }

    inline void start(const PAny& payload) override {
        InitEntry(payload);
    }

private:
    enum {
        Init = 1
    };

    /* region Entry Methods */
    inline void InitEntry(const PAny& payload) {
        states.setTop(Init);
        InitEntryImpl();
    }
    /* end Entry Methods */

    /* region Transition Methods */
    /* end Transition Methods */

    /* region Function Implementations */
    inline void InitEntryImpl() {
        PMachine* coor;
        int index;
        auto&& tmp2 = create<MachineCoordinateMachine>();
        coor = tmp2;
        auto&& tmp4 = 0;
        index = tmp4;
        auto&& tmp5 = index;
        auto&& tmp6 = 5;
        auto&& tmp7 = tmp5 < tmp6;
        while(tmp7) {
            auto&& tmp8 = randomBool();
            if(tmp8) {
                auto&& tmp9 = coor;
                auto&& tmp10 = eTransaction;
                auto&& tmp11 = index;
                send(tmp9, tmp10, tmp11);
            }
            auto&& tmp13 = index;
            auto&& tmp14 = 1;
            auto&& tmp15 = tmp13 + tmp14;
            index = tmp15;
            auto&& tmp5 = index;
            auto&& tmp6 = 5;
            tmp7 = tmp5 < tmp6;
        }
    }
    /* end Function Implementations */

    /* region Machine Fields */
    /* end Machine Fields  */

    /* region Jump Tables */
    inline bool isDefered(int state, int event) const override {
        static const bool _isDefered[2][8] = 
            {
                { true, true, true, true, true, true, true, true} /* halt */,
                { true,false,false,false,false,false,false,false} /* Init */
            };
        return _isDefered[state][event];
    }

    inline bool isGotoTransition(int state, int event) const override {
        static const bool _isGotoTransition[2][8] =
            {
                {false,false,false,false,false,false,false,false} /* halt */,
                {false, true,false,false,false,false,false,false} /* Init */
            };
        return _isGotoTransition[state][event];
    }

    inline ExitFunction getExitFunction(int state) const override {
        #define E(f) ((ExitFunction)&MachineMain::f)
        static ExitFunction _exitFunctions[] = {&MachineMain::emptyExit,&MachineMain::emptyExit};
        #undef E
        return _exitFunctions[state];
    }

    inline TransitionFunction getTransition(int state, int event) const override {
        #define E(f) ((TransitionFunction)&MachineMain::f)
        static TransitionFunction _transitions[2][8] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL}
            };
        #undef E
        return _transitions[state][event];
    }

    inline EntryFunction getTransitionEntry(int state, int event) const override {
        #define E(f) ((TransitionFunction)&MachineMain::f)
        static TransitionFunction _entries[2][8] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,NULL,NULL,NULL,NULL}
            };
        #undef E
        return _entries[state][event];
    }
    /* end Jump Tables */
};

};