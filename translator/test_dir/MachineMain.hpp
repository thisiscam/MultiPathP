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
        Init = 1,
        Receive = 2
    };

    /* region Entry Methods */
    static inline void InitEntry(MachineMain* self, const PAny& payload) {
        self->states.setTop(Init);
        self->InitEntryImpl();
    }

    static inline void ReceiveEntry(MachineMain* self, const PAny& payload) {
        self->states.setTop(Receive);
        self->ReceiveEntryImpl(payload);
    }
    /* end Entry Methods */

    /* region Transition Methods */
    /* end Transition Methods */

    /* region Function Implementations */
    inline VOID_FUNCTION_DECL(InitEntryImpl, ()) {
        Int i;
        Int v;
        v = 1;
        WHILE(i < 5) {
            create<MachineNode>(PTuple<Ptr<PMachine>, Int>(self(), v));
            i = i + 1;
            v = v * 2;
        }
        ENDWHILE()
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(ReceiveEntryImpl, (Int msg)) {
        IF(x < msg) 
        THEN() {
            x = x + msg;
        }
        ENDIF()
        y = y + msg;
    }
    END_VOID_FUNCTION()
    /* end Function Implementations */

    /* region Machine Fields */
    Int x;
    Int y;
    /* end Machine Fields  */

    /* region Jump Tables */
    inline Bool isDefered(const Int& state, const Int& event) const override {
        static const bool _isDefered[3][3] = 
            {
                { true, true, true} /* halt */,
                { true,false,false} /* Init */,
                { true,false,false} /* Receive */
            };
        return getIndex2D(_isDefered, state, event);
    }

    inline Bool isGotoTransition(const Int& state, const Int& event) const override {
        static const bool _isGotoTransition[3][3] =
            {
                {false,false,false} /* halt */,
                {false, true, true} /* Init */,
                {false, true, true} /* Receive */
            };
        return getIndex2D(_isGotoTransition, state, event);
    }

    inline ExitFunctionPtr getExitFunction(const Int& state) const override {
        #define E(f) ((ExitFunction)&MachineMain::f)
        static ExitFunction _exitFunctions[] = {&MachineMain::emptyExit,&MachineMain::emptyExit,&MachineMain::emptyExit};
        #undef E
        return getIndex1D(_exitFunctions, state);
    }

    inline TransitionFunctionPtr getTransition(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineMain::f)
        static TransitionFunction _transitions[3][3] = 
            {
                {NULL,NULL,NULL},
                {NULL,E(emptyTransition),E(emptyTransition)},
                {NULL,E(emptyTransition),E(emptyTransition)}
            };
        #undef E
        return getIndex2D(_transitions, state, event);
    }

    inline EntryFunctionPtr getTransitionEntry(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineMain::f)
        static TransitionFunction _entries[3][3] = 
            {
                {NULL,NULL,NULL},
                {NULL,E(haltEntry),E(ReceiveEntry)},
                {NULL,E(haltEntry),E(ReceiveEntry)}
            };
        #undef E
        return getIndex2D(_entries, state, event);
    }
    /* end Jump Tables */
};

};