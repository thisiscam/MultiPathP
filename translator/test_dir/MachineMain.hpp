namespace pingpong {

class MachineMain final : public PMachine {
    friend class Scheduler;
public:
    MachineMain(ExecutionEngine& engine):PMachine(engine) { }

    inline void start(const PAny& payload) override {
        InitEntry(this, payload);
    }

private:
    enum {
        Init = 1
    };

    /* region Entry Methods */
    static inline void InitEntry(MachineMain* self, const PAny& payload) {
        self->states.setTop(Init);
        self->InitEntryImpl();
    }
    /* end Entry Methods */

    /* region Transition Methods */
    /* end Transition Methods */

    /* region Function Implementations */
    inline VOID_FUNCTION_DECL(InitEntryImpl, ()) {
        Ptr<PMachine> coor;
        Int index;
        coor = create<MachineCoordinateMachine>();
        IF(randomBool("0")) 
        THEN() {
            send(coor, eTransaction);
        }
        ENDIF()
        IF(randomBool("1")) 
        THEN() {
            send(coor, eTransaction);
        }
        ENDIF()
        IF(randomBool("2")) 
        THEN() {
            send(coor, eTransaction);
        }
        ENDIF()
    }
    END_VOID_FUNCTION()
    /* end Function Implementations */

    /* region Machine Fields */
    /* end Machine Fields  */

    /* region Jump Tables */
    inline Bool isDefered(const Int& state, const Int& event) const override {
        static const bool _isDefered[2][7] = 
            {
                { true, true, true, true, true, true, true} /* halt */,
                { true,false,false,false,false,false,false} /* Init */
            };
        return getIndex2D(_isDefered, state, event);
    }

    inline Bool isGotoTransition(const Int& state, const Int& event) const override {
        static const bool _isGotoTransition[2][7] =
            {
                {false,false,false,false,false,false,false} /* halt */,
                {false, true,false,false,false,false,false} /* Init */
            };
        return getIndex2D(_isGotoTransition, state, event);
    }

    inline ExitFunctionPtr getExitFunction(const Int& state) const override {
        #define E(f) ((ExitFunction)&MachineMain::f)
        static ExitFunction _exitFunctions[] = {&MachineMain::emptyExit,&MachineMain::emptyExit};
        #undef E
        return getIndex1D(_exitFunctions, state);
    }

    inline TransitionFunctionPtr getTransition(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineMain::f)
        static TransitionFunction _transitions[2][7] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL}
            };
        #undef E
        return getIndex2D(_transitions, state, event);
    }

    inline EntryFunctionPtr getTransitionEntry(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineMain::f)
        static TransitionFunction _entries[2][7] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,NULL,NULL,NULL}
            };
        #undef E
        return getIndex2D(_entries, state, event);
    }
    /* end Jump Tables */
};

};