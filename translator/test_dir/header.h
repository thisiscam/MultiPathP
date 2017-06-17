#ifndef pingpong_H
#define pingpong_H

#define DECL_TYPES List<PMachine*, int, int, PList<PMachine*>, bool, PTuple<int, PMachine*>>
#include "basic_cpp_runtime.h"

using namespace basic_cpp_runtime;

namespace pingpong {

const int eUnit = 2;
const int eTransaction = 3;
const int eCommit = 4;
const int eAbort = 5;
const int eSuccess = 6;
const int eFailure = 7;

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

	static inline PMachine* createMainMachine(ExecutionEngine& engine) {
		return new MachineMain(engine);
	}

	static inline void announce(int event, const PAny& payload) {

	}
};

}

#endif