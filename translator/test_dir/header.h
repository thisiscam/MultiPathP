#ifndef pingpong_H
#define pingpong_H

#define DECL_TYPES List<PTuple<Int, PTuple<Int, Int>, Int, Int>, PTuple<Int, Int>, PList<Ptr<PMachine>>, PTuple<Int>, PMap<Int, PTuple<PTuple<Int, Int>, Int>>, Bool, PTuple<Int, PTuple<Int, Int>, Int>, PTuple<Int, PTuple<Int, Int>>, Ptr<PMachine>, PTuple<PTuple<Int, Int>, Int>, PTuple<Ptr<PMachine>, Int, PTuple<Int, Int>, Int>, PTuple<Ptr<PMachine>, Int, PTuple<Int, Int>>, Int, Int, PTuple<PList<Ptr<PMachine>>>, PTuple<PList<Ptr<PMachine>>, Ptr<PMachine>, Int>, PTuple<Int, Ptr<PMachine>>, PTuple<Ptr<PMachine>, Int>>
#include "PCppRuntime.h"

using namespace RUNTIME_NAMESPACE;

namespace pingpong {

const int prepare = 2;
const int accept = 3;
const int agree = 4;
const int reject = 5;
const int accepted = 6;
const int local = 7;
const int success = 8;
const int allNodes = 9;
const int goPropose = 10;
const int chosen = 11;
const int update = 12;
const int announce_valueChosen = 13;
const int announce_valueProposed = 14;
const int announce_client_sent = 15;
const int announce_proposer_sent = 16;
const int announce_proposer_chosen = 17;
const int Ping = 18;
const int newLeader = 19;
const int timeout = 20;
const int startTimer = 21;
const int cancelTimer = 22;
const int cancelTimerSuccess = 23;
const int response = 24;

class MachinePaxosNode;
class MachineLeaderElection;
class MachineTimer;
class MachineMain;
class MachineClient;

class Controller {
using AnnounceHandlerPtr = void (*)(const PAny&);
public:

	static inline Ptr<PMachine> createMainMachine(ExecutionEngine& engine) {
		return PMachine::alloc<MachineMain>(engine);
	}

    static inline void announceEmpty(const PAny& payload) { }

    /* region Announce Handler Implementations */
    /* end Announce Handler Implemntatons */

	static inline void announce(const Int& event, const PAny& payload) {
        static AnnounceHandlerPtr _handlers[25] =
        {
            announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty
        };
        auto&& ptr = getIndex1D(_handlers, event);
        return INVOKE_PTR(ptr, void, (payload));
	}

private:
};

}

#include "MachinePaxosNode.hpp"
#include "MachineLeaderElection.hpp"
#include "MachineTimer.hpp"
#include "MachineMain.hpp"
#include "MachineClient.hpp"

#endif