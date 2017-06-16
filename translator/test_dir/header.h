#ifndef pingpong_H
#define pingpong_H

#define DECL_TYPES List<PMachine*, Event>
#include "basic_cpp_runtime.h"

using namespace basic_cpp;

namespace pingpong {

const int Ping = 2;
const int Pong = 3;
const int Success = 4;

class MachinePING;
class MachinePONG;

};

#include "MachinePING.hpp"
#include "MachinePONG.hpp"

namespace pingpong {

class Controller {
public:
	static inline PMachine* createMainMachine(ExecutionEngine& engine) {
		return new MachinePING();
	}
};

}

#endif