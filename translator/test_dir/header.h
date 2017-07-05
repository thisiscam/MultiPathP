#ifndef pingpong_H
#define pingpong_H

#define DECL_TYPES List<Ptr<PMachine>, Int, PTuple<Int, Int>, Int, PList<Ptr<PMachine>>, Bool, PTuple<Int, Int, Int>, PMap<Int, Int>, PTuple<Int, Ptr<PMachine>>, PTuple<Ptr<PMachine>, Int>, PTuple<Ptr<PMachine>, Int, Int>>
#include "PCppRuntime.h"

using namespace RUNTIME_NAMESPACE;

namespace pingpong {

const int REQ_REPLICA = 2;
const int RESP_REPLICA_COMMIT = 3;
const int RESP_REPLICA_ABORT = 4;
const int GLOBAL_ABORT = 5;
const int GLOBAL_COMMIT = 6;
const int WRITE_REQ = 7;
const int WRITE_FAIL = 8;
const int WRITE_SUCCESS = 9;
const int READ_REQ = 10;
const int READ_UNAVAILABLE = 11;
const int READ_SUCCESS = 12;
const int Unit = 13;
const int Timeout = 14;
const int StartTimer = 15;
const int CancelTimer = 16;
const int CancelTimerFailure = 17;
const int CancelTimerSuccess = 18;
const int MONITOR_WRITE = 19;
const int MONITOR_READ_SUCCESS = 20;
const int MONITOR_READ_UNAVAILABLE = 21;

class MachineTimer;
class MachineReplica;
class MachineCoordinator;
class MachineClient;
class MachineM;
class MachineTwoPhaseCommit;

class Controller {
using AnnounceHandlerPtr = void (*)(const PAny&);
public:

	static inline Ptr<PMachine> createMainMachine(ExecutionEngine&);

    static inline void announceEmpty(const PAny& payload) { }

    static inline void AnnounceMONITOR_WRITEHandler(const PAny& payload) {
        getMachineMInstance()->serveEvent(MONITOR_WRITE, payload);
    }
    static inline void AnnounceMONITOR_READ_SUCCESSHandler(const PAny& payload) {
        getMachineMInstance()->serveEvent(MONITOR_READ_SUCCESS, payload);
    }
    static inline void AnnounceMONITOR_READ_UNAVAILABLEHandler(const PAny& payload) {
        getMachineMInstance()->serveEvent(MONITOR_READ_UNAVAILABLE, payload);
    }

	static inline void announce(const Int& event, const PAny& payload) {
        static AnnounceHandlerPtr _handlers[22] =
        {
            announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,announceEmpty,AnnounceMONITOR_WRITEHandler,AnnounceMONITOR_READ_SUCCESSHandler,AnnounceMONITOR_READ_UNAVAILABLEHandler
        };
        auto&& ptr = getIndex1D(_handlers, event);
        return INVOKE_PTR(ptr, void, (payload));
	}

private:
    static inline PMachine*& getMachineMInstance() {
        static PMachine* _m = nullptr;
        return _m;
    }
};

}

#include "MachineTimer.hpp"
#include "MachineReplica.hpp"
#include "MachineCoordinator.hpp"
#include "MachineClient.hpp"
#include "MachineM.hpp"
#include "MachineTwoPhaseCommit.hpp"

inline Ptr<PMachine> 
pingpong::Controller::createMainMachine(ExecutionEngine& engine) {
    getMachineMInstance() = new MachineM(engine);
    getMachineMInstance()->start();
    return PMachine::alloc<MachineTwoPhaseCommit>(engine);
}
#endif