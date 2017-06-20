namespace pingpong {

class MachinePONG final : public PMachine {
    friend class Scheduler;
public:
    MachinePONG(ExecutionEngine& engine):PMachine(engine) { }

    inline void start(const PAny& payload) override {
        Pong_WaitPingEntry(payload);
    }

private:
    enum {
        Pong_WaitPing = 1,
        Pong_SendPong = 2
    };

    /* region Entry Methods */
    inline void Pong_WaitPingEntry(const PAny& payload) {
        states.setTop(Pong_WaitPing);
        Pong_WaitPingEntryImpl();
    }

    inline void Pong_SendPongEntry(const PAny& payload) {
        states.setTop(Pong_SendPong);
        Pong_SendPongEntryImpl(static_cast<Ptr<PMachine>>(payload));
    }
    /* end Entry Methods */

    /* region Transition Methods */
    /* end Transition Methods */

    /* region Function Implementations */
    inline void Pong_WaitPingEntryImpl() {

    }

    inline void Pong_SendPongEntryImpl(Ptr<PMachine> payload) {
        auto&& tmp9 = payload;
        auto&& tmp10 = Pong;
        send(tmp9, tmp10);
        auto&& tmp11 = Success;
        raise(tmp11); retcode = RAISED_EVENT; return;
    }
    /* end Function Implementations */

    /* region Machine Fields */
    /* end Machine Fields  */

    /* region Jump Tables */
    inline bool isDefered(int state, int event) const override {
        static const bool _isDefered[3][5] = 
            {
                { true, true, true, true, true} /* halt */,
                { true,false,false,false,false} /* Pong_WaitPing */,
                { true,false,false,false,false} /* Pong_SendPong */
            };
        return _isDefered[state][event];
    }

    inline bool isGotoTransition(int state, int event) const override {
        static const bool _isGotoTransition[3][5] =
            {
                {false,false,false,false,false} /* halt */,
                {false, true, true,false,false} /* Pong_WaitPing */,
                {false, true,false,false, true} /* Pong_SendPong */
            };
        return _isGotoTransition[state][event];
    }

    inline ExitFunction getExitFunction(int state) const override {
        #define E(f) ((ExitFunction)&MachinePONG::f)
        static ExitFunction _exitFunctions[] = {&MachinePONG::emptyExit,&MachinePONG::emptyExit,&MachinePONG::emptyExit};
        #undef E
        return _exitFunctions[state];
    }

    inline TransitionFunction getTransition(int state, int event) const override {
        #define E(f) ((TransitionFunction)&MachinePONG::f)
        static TransitionFunction _transitions[3][5] = 
            {
                {NULL,NULL,NULL,NULL,NULL},
                {NULL,E(emptyTransition),E(emptyTransition),NULL,NULL},
                {NULL,E(emptyTransition),NULL,NULL,E(emptyTransition)}
            };
        #undef E
        return _transitions[state][event];
    }

    inline EntryFunction getTransitionEntry(int state, int event) const override {
        #define E(f) ((TransitionFunction)&MachinePONG::f)
        static TransitionFunction _entries[3][5] = 
            {
                {NULL,NULL,NULL,NULL,NULL},
                {NULL,E(haltEntry),E(Pong_SendPongEntry),NULL,NULL},
                {NULL,E(haltEntry),NULL,NULL,E(Pong_WaitPingEntry)}
            };
        #undef E
        return _entries[state][event];
    }
    /* end Jump Tables */
};

};