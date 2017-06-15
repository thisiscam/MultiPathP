#include "header.h"

namespace  {

class MachinePING final : public PMachine {

public:

    inline void start(const PAny& payload) override {
        Ping_InitEntry(payload);
    }

private:

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
        pongId = create<PONG>();;
        raise(Success);
    }
    inline void Ping_SendPingEntryImpl() {
        send(pongId, Ping, this);
        raise(Success);
    }
    /* end Function Implementations */

    /* region Machine Fields */
    PMachine* pongId = NULL;
    /* end Machine Fields  */

    /* region Jump Tables */
    inline bool isDefered(int state, int event) const override {
        static const bool _isDefered[][] = {{false,false,false,false},{false,false,false,false},{false,false,false,false}};
        return _isDefered[state][event];
    }

    inline bool isGotoTransition(int state, int event) const override {
        static const bool _isGotoTransition[][] = {{false,false,false,false},{false,false,false,false},{false,false,false,false}};
        return _isGotoTransition[state][event];
    }

    inline ExitFunction getExitFunction(int state) const override {
        static ExitFunction _exitFunctions[] = {&exitIgnore,&exitIgnore,&exitIgnore,&exitIgnore};
        return _exitFunctions[state];
    }

    inline TransitionFunction getTransition(int state, int event) const override {
        static TransitionFunction _transitions[][] = {};
        return _transitions[state][event];
    }
    /* end Jump Tables */
}