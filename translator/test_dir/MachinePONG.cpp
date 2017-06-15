#include "header.h"

namespace  {

class MachinePONG final : public PMachine {

public:

    inline void start(const PAny& payload) override {
        Pong_WaitPingEntry(payload);
    }

private:

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
        send(payload, Pong);
        raise(Success);
    }
    /* end Function Implementations */

    /* region Machine Fields */
    /* end Machine Fields  */

    /* region Jump Tables */
    inline bool isDefered(int state, int event) const override {
        static const bool _isDefered[][] = {{false,false},{false,false},{false,false}};
        return _isDefered[state][event];
    }

    inline bool isGotoTransition(int state, int event) const override {
        static const bool _isGotoTransition[][] = {{false,false},{false,false},{false,false}};
        return _isGotoTransition[state][event];
    }

    inline ExitFunction getExitFunction(int state) const override {
        static ExitFunction _exitFunctions[] = {&exitIgnore,&exitIgnore};
        return _exitFunctions[state];
    }

    inline TransitionFunction getTransition(int state, int event) const override {
        static TransitionFunction _transitions[][] = {};
        return _transitions[state][event];
    }
    /* end Jump Tables */
}