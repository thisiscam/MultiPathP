namespace pingpong {

class MachinePING final : public PMachine {
    friend class Scheduler;
public:
    MachinePING(ExecutionEngine& engine):PMachine(engine) { }

    inline void start(const PAny& payload) override {
        Ping_InitEntry(payload);
    }

private:
    static const int Ping_Init = 1;
    static const int Ping_SendPing = 2;
    static const int Ping_WaitPong = 3;
    static const int Done = 4;

    /* region Entry Methods */
    inline void Ping_InitEntry(const PAny& payload) {
        states.set(states.size(), Ping_Init);
        Ping_InitEntryImpl();
    }

    inline void Ping_SendPingEntry(const PAny& payload) {
        states.set(states.size(), Ping_SendPing);
        Ping_SendPingEntryImpl();
    }

    inline void Ping_WaitPongEntry(const PAny& payload) {
        states.set(states.size(), Ping_WaitPong);
    }

    inline void DoneEntry(const PAny& payload) {
        states.set(states.size(), Done);
    }
    /* end Entry Methods */

    /* region Transition Methods */
    /* end Transition Methods */

    /* region Function Implementations */
    inline void Ping_InitEntryImpl() {
        auto&& tmp2 = create<MachinePONG>();
        pongId = tmp2;
        auto&& tmp4 = Success;
        raise(tmp4); retcode = RAISED_EVENT; return;
    }

    inline void Ping_SendPingEntryImpl() {
        auto&& tmp5 = pongId;
        auto&& tmp6 = Ping;
        auto&& tmp7 = this;
        send(tmp5, tmp6, tmp7);
        auto&& tmp8 = Success;
        raise(tmp8); retcode = RAISED_EVENT; return;
    }
    /* end Function Implementations */

    /* region Machine Fields */
    PMachine* pongId = NULL;
    /* end Machine Fields  */

    /* region Jump Tables */
    inline bool isDefered(int state, int event) const override {
        static const bool _isDefered[][] = {{false,false,false,false,false},{false,false,false,false,false},{false,false,false,false,false},{false,false,false,false,false},{false,false,false,false,false}};
        return _isDefered[state][event];
    }

    inline bool isGotoTransition(int state, int event) const override {
        static const bool _isGotoTransition[][] = {{false,false,false,false,false},{false,false,false,false,false},{false,false,false,false,false},{false,false,false,false,false},{false,false,false,false,false}};
        return _isGotoTransition[state][event];
    }

    inline ExitFunction getExitFunction(int state) const override {
        static ExitFunction _exitFunctions[] = {&exitIgnore,&exitIgnore,&exitIgnore,&exitIgnore,&exitIgnore};
        return _exitFunctions[state];
    }

    inline TransitionFunction getTransition(int state, int event) const override {
        static TransitionFunction _transitions[][] = {{&emptyTransition,NULL,NULL,NULL,NULL},{&emptyTransition,&emptyTransition,&emptyTransition,&emptyTransition,&emptyTransition},{&emptyTransition,NULL,NULL,NULL,NULL},{&emptyTransition,NULL,NULL,&emptyTransition,NULL},{&emptyTransition,&emptyTransition,&emptyTransition,NULL,NULL}};
        return _transitions[state][event];
    }

    inline EntryFunction getTransitionEntry(int state, int event) const override {
        static TransitionFunction _entries[][] = {{&emptyEntry,NULL,NULL,NULL,NULL},{&emptyEntry,&haltEntry,&haltEntry,&haltEntry,&haltEntry},{&emptyEntry,NULL,NULL,NULL,NULL},{&emptyEntry,NULL,NULL,&Ping_SendPingEntry,NULL},{&emptyEntry,&Ping_SendPingEntry,&Ping_WaitPongEntry,NULL,NULL}};
        return _entries[state][event];
    }
    /* end Jump Tables */
}