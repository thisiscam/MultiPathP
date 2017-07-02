#ifndef pingpong_H
#define pingpong_H

#define DECL_TYPES List<Ptr<PMachine>, Int, Bool>
#include "PCppRuntime.h"

using namespace RUNTIME_NAMESPACE;

namespace pingpong {

const int eUnit = 2;
const int eTransaction = 3;
const int eCommit = 4;
const int eSuccess = 5;
const int eFailure = 6;

class MachineMain;
class MachineCoordinateMachine;
class MachineParticipantMachine;

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
        static AnnounceHandlerPtr _handlers[7] =
        {
            announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty
        };
        auto&& ptr = getIndex1D(_handlers, event);
        return INVOKE_PTR(ptr, void, (payload));
	}

private:
};

}

#include "MachineMain.hpp"
#include "MachineCoordinateMachine.hpp"
#include "MachineParticipantMachine.hpp"

#endif