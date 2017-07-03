namespace pingpong {

class MachineM final : public PMachine {
    friend class Scheduler;
public:
    MachineM(ExecutionEngine& engine):PMachine(engine) { }

    inline void start(const PAny& payload) override {
        InitEntry(this, payload);
    }

private:
    enum {
        Init = 1
    };

    /* region Entry Methods */
    static inline void InitEntry(MachineM* self, const PAny& payload) {
        self->states.setTop(Init);
    }
    /* end Entry Methods */

    /* region Transition Methods */
    static inline void HandlerImpl10_Wrap(MachineM* self, const PAny& payload) {
        self->HandlerImpl10(payload);
    }

    static inline void HandlerImpl12_Wrap(MachineM* self, const PAny& payload) {
        self->HandlerImpl12(payload);
    }

    static inline void HandlerImpl11_Wrap(MachineM* self, const PAny& payload) {
        self->HandlerImpl11(payload);
    }
    /* end Transition Methods */

    /* region Function Implementations */
    inline VOID_FUNCTION_DECL(HandlerImpl10, (PTuple<Int, Int> payload)) {
        data.set(payload.v1(), payload.v0());
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(HandlerImpl11, (PTuple<Int, Int> payload)) {
        passert((data.containsKey(payload.v0())), "");
        passert((data.get(payload.v0()) == payload.v1()), "");
    }
    END_VOID_FUNCTION()

    inline VOID_FUNCTION_DECL(HandlerImpl12, (Int payload)) {
        passert((!(data.containsKey(payload))), "");
    }
    END_VOID_FUNCTION()
    /* end Function Implementations */

    /* region Machine Fields */
    PMap<Int, Int> data;
    /* end Machine Fields  */

    /* region Jump Tables */
    inline Bool isDefered(const Int& state, const Int& event) const override {
        static const bool _isDefered[2][22] = 
            {
                { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true} /* halt */,
                { true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* Init */
            };
        return getIndex2D(_isDefered, state, event);
    }

    inline Bool isGotoTransition(const Int& state, const Int& event) const override {
        static const bool _isGotoTransition[2][22] =
            {
                {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* halt */,
                {false, true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false} /* Init */
            };
        return getIndex2D(_isGotoTransition, state, event);
    }

    inline ExitFunctionPtr getExitFunction(const Int& state) const override {
        #define E(f) ((ExitFunction)&MachineM::f)
        static ExitFunction _exitFunctions[] = {&MachineM::emptyExit,&MachineM::emptyExit};
        #undef E
        return getIndex1D(_exitFunctions, state);
    }

    inline TransitionFunctionPtr getTransition(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineM::f)
        static TransitionFunction _transitions[2][22] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(HandlerImpl10_Wrap),E(HandlerImpl11_Wrap),E(HandlerImpl12_Wrap)}
            };
        #undef E
        return getIndex2D(_transitions, state, event);
    }

    inline EntryFunctionPtr getTransitionEntry(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineM::f)
        static TransitionFunction _entries[2][22] = 
            {
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,E(emptyEntry),E(emptyEntry),E(emptyEntry)}
            };
        #undef E
        return getIndex2D(_entries, state, event);
    }
    /* end Jump Tables */
};

};