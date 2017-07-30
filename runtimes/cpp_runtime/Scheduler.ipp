#ifndef SCHEDULE_IPP
#define SCHEDULE_IPP

#include <typeinfo>
#include "PMachine.hpp"
#include "Helpers.h"
#include <valgrind/callgrind.h>

namespace RUNTIME_NAMESPACE {

inline const PList<SendQueueItem>&
Scheduler::getSendQueue(PMachine* machine) {
    return machine->sendQueue;
}

#ifdef USE_VALUE_SUMMARY
static std::ostream& operator<<(std::ostream& os, const Ptr<PMachine>& m) {
    os << "VS[";
    int count = 0;
    unaryOp<void>(m, [&](PMachine* m) {
        if(count > 0) {
            os << ",";
        }
        os << *m;
        count++;
    });
    os << "]";
    return os;
}
#else
static std::ostream& operator<<(std::ostream& os, const Ptr<PMachine>& m) {
    os << *m;
    return os;
}
#endif

inline SendQueueItem
Scheduler::popSendQueueItem(const Ptr<PMachine>& machine, const Int& index) {
    return unaryOp<SendQueueItem>(machine, [&](PMachine* machine) {
        Int i = index;
        SendQueueItem&& item = machine->sendQueue.get(i);
        machine->sendQueue.removeAt(i);
        return item;
    });
}

inline FUNCTION_DECL(Bool, Scheduler::step, ()) {
    SchedulerChoice&& chosen = chooseMachine();
    IF(chosen.machine == NULL)
    THEN() {
        RETURN(false);
    }
    ELSE() {
        IF(chosen.queueIdx < 0)
        THEN() {
            std::cout << chosen.machine << " executes null event" << std::endl;
            INVOKE(chosen.machine, void, step, (chosen.stateIdx, EVENT_NULL));
        }
        ELSE() {
            SendQueueItem&& item = popSendQueueItem(chosen.machine, chosen.queueIdx);
            IF(item.e == EVENT_NEW_MACHINE)
            THEN() {
                std::cout << Ptr<PMachine>(chosen.machine) << " creates " << Ptr<PMachine>(item.target) << std::endl;
                startMachine(item.target, item.payload);
            }
            ELSE() {
                std::cout << Ptr<PMachine>(chosen.machine) << " sends event " << PEvent(item.e) << " to " << Ptr<PMachine>(item.target) << std::endl;
                INVOKE(item.target, void, step, (chosen.stateIdx, item.e, item.payload));
            }
            ENDIF()
        }
        ENDIF()
        RETURN(true);
    }
    ENDIF()
}
END_FUNCTION()

}

#endif