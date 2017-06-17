#ifndef SCHEDULE_IPP
#define SCHEDULE_IPP

#include "PMachine.hpp"

namespace basic_cpp_runtime {

inline PList<SendQueueItem>&
Scheduler::getSendQueue(PMachine* machine) {
    return machine->sendQueue;
}

inline bool 
Scheduler::step() {
    SchedulerChoice&& chosen = chooseMachine();
    if(chosen.machine == NULL) {
        return false;
    } else {
        if(chosen.queueIdx < 0) {
            std::cout << chosen.machine << " executes null event" << std::endl;
            chosen.machine->step(chosen.stateIdx, EVENT_NULL);
        } else {
            SendQueueItem item = getSendQueue(chosen.machine).get(chosen.queueIdx);
            getSendQueue(chosen.machine).removeAt(chosen.queueIdx);
            if(item.e == EVENT_NEW_MACHINE) {
                std::cout << chosen.machine << " creates " << item.target << std::endl;
                startMachine(item.target, item.payload);
            } else {
                std::cout << chosen.machine << " sends event " << item.e << " to " << item.target << std::endl;
                item.target->step(chosen.stateIdx, item.e, item.payload);
            }
        }
        return true;
    }
}

}

#endif