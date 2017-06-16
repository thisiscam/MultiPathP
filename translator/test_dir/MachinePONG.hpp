namespace pingpong {

class MachinePONG final : public PMachine {
    friend class Scheduler;
public:
    MachinePONG(ExecutionEngine& engine):PMachine(engine) { }

    inline void start(const PAny& payload) override {
        Pong_WaitPingEntry(payload);
    }

private:
    static const int Pong_WaitPing = 1;
    static const int Pong_SendPong = 2;

    /* region Entry Methods */
    inline void Pong_WaitPingEntry(const PAny& payload) {
        states.set(states.size(), Pong_WaitPing);
        Pong_WaitPingEntryImpl();
    }

    inline void Pong_SendPongEntry(const PAny& payload) {
        states.set(states.size(), Pong_SendPong);
        Pong_SendPongEntryImpl(cast(com.multipathp.pprogram.types.PType@44543d5, payload));
    }
    /* end Entry Methods */

    /* region Transition Methods */
    /* end Transition Methods */

    /* region Function Implementations */
    inline void Pong_WaitPingEntryImpl() {
    }

    inline void Pong_SendPongEntryImpl(PMachine* payload) {
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
        static const bool _isDefered[][] = {{false,false,false},{false,false,false},{false,false,false},{false,false,false},{false,false,false}};
        return _isDefered[state][event];
    }

    inline bool isGotoTransition(int state, int event) const override {
        static const bool _isGotoTransition[][] = {{false,false,false},{false,false,false},{false,false,false},{false,false,false},{false,false,false}};
        return _isGotoTransition[state][event];
    }

    inline ExitFunction getExitFunction(int state) const override {
        static ExitFunction _exitFunctions[] = {&exitIgnore,&exitIgnore,&exitIgnore};
        return _exitFunctions[state];
    }

    inline TransitionFunction getTransition(int state, int event) const override {
        static TransitionFunction _transitions[][] = {{&emptyTransition,NULL,NULL},{&emptyTransition,&emptyTransition,&emptyTransition},{&emptyTransition,&emptyTransition,NULL},{&emptyTransition,NULL,NULL},{&emptyTransition,NULL,&emptyTransition}};
        return _transitions[state][event];
    }

    inline EntryFunction getTransitionEntry(int state, int event) const override {
        static TransitionFunction _entries[][] = {{&emptyEntry,NULL,NULL},{&emptyEntry,&haltEntry,&haltEntry},{&emptyEntry,&Pong_SendPongEntry,NULL},{&emptyEntry,NULL,NULL},{&emptyEntry,NULL,&Pong_WaitPingEntry}};
        return _entries[state][event];
    }
    /* end Jump Tables */
}