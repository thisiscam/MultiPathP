#ifndef pingpong_H
#define pingpong_H

#define DECL_TYPES List<Ptr<PMachine>, Int, Bool>
#include "basic_cpp_runtime.h"

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

};

#include "MachineMain.hpp"
#include "MachineCoordinateMachine.hpp"
#include "MachineParticipantMachine.hpp"

namespace pingpong {

class Controller {
public:

	static inline Ptr<PMachine> createMainMachine(ExecutionEngine& engine) {
		return new MachineMain(engine);
	}

	static inline void announce(const Int& event, const PAny& payload) {

	}
};

}

#endif