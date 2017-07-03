#ifndef pingpong_H
#define pingpong_H

#define DECL_TYPES List<Ptr<PMachine>, Int, Int, Bool, PTuple<Ptr<PMachine>, Int>>
#include "PCppRuntime.h"

using namespace RUNTIME_NAMESPACE;

namespace pingpong {

const int eEchoBack = 2;

class MachineMain;
class MachineNode;

class Controller {
using AnnounceHandlerPtr = void (*)(const PAny&);
public:

	static inline Ptr<PMachine> createMainMachine(ExecutionEngine&);

    static inline void announceEmpty(const PAny& payload) { }


	static inline void announce(const Int& event, const PAny& payload) {
        static AnnounceHandlerPtr _handlers[3] =
        {
            announceEmpty,announceEmpty,announceEmpty
        };
        auto&& ptr = getIndex1D(_handlers, event);
        return INVOKE_PTR(ptr, void, (payload));
	}

private:
};

}

#include "MachineMain.hpp"
#include "MachineNode.hpp"

inline Ptr<PMachine> 
pingpong::Controller::createMainMachine(ExecutionEngine& engine) {
    return PMachine::alloc<MachineMain>(engine);
}
#endif