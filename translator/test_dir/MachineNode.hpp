namespace pingpong {

class MachineNode final : public PMachine {
    friend class Scheduler;
public:
    MachineNode(ExecutionEngine& engine):PMachine(engine) { }

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
        InitEntryImpl(payload);
    }
    /* end Entry Methods */

    /* region Transition Methods */
    /* end Transition Methods */

    /* region Function Implementations */
    inline FUNCTION_DECL(void, InitEntryImpl, (PTuple<Ptr<PMachine>, Int> payload), {
        send(payload.v0(), eEchoBack, payload.v1());
    })
    /* end Function Implementations */

    /* region Machine Fields */
    /* end Machine Fields  */

    /* region Jump Tables */
    inline Bool isDefered(const Int& state, const Int& event) const override {
        static const bool _isDefered[2][3] = 
            {
                { true, true, true} /* halt */,
                { true,false,false} /* Init */
            };
        return getIndex2D(_isDefered, state, event);
    }

    inline Bool isGotoTransition(const Int& state, const Int& event) const override {
        static const bool _isGotoTransition[2][3] =
            {
                {false,false,false} /* halt */,
                {false, true,false} /* Init */
            };
        return getIndex2D(_isGotoTransition, state, event);
    }

    inline ExitFunctionPtr getExitFunction(const Int& state) const override {
        #define E(f) ((ExitFunction)&MachineNode::f)
        static ExitFunction _exitFunctions[] = {&MachineNode::emptyExit,&MachineNode::emptyExit};
        #undef E
        return getIndex1D(_exitFunctions, state);
    }

    inline TransitionFunctionPtr getTransition(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineNode::f)
        static TransitionFunction _transitions[2][3] = 
            {
                {NULL,NULL,NULL},
                {NULL,E(emptyTransition),NULL}
            };
        #undef E
        return getIndex2D(_transitions, state, event);
    }

    inline EntryFunctionPtr getTransitionEntry(const Int& state, const Int& event) const override {
        #define E(f) ((TransitionFunction)&MachineNode::f)
        static TransitionFunction _entries[2][3] = 
            {
                {NULL,NULL,NULL},
                {NULL,E(haltEntry),NULL}
            };
        #undef E
        return getIndex2D(_entries, state, event);
    }
    /* end Jump Tables */
};

};