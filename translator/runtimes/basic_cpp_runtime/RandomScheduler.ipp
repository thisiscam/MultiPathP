#ifndef RANDOM_SCHEDULER_IPP
#define RANDOM_SCHEDULER_IPP

#include "PMachine.hpp"

namespace basic_cpp_runtime {

inline SchedulerChoice 
RandomScheduler::chooseMachine() {
	PList<SchedulerChoice> choices;
	for(int i=0; i < machines.size(); i++) {
        PMachine* machine = machines.get(i);
        // Collect from send queue
        PList<SendQueueItem>& sendQueue = getSendQueue(machine);
        bool doLoop = true;
		for(int j=0; doLoop && j < sendQueue.size(); j++) {
            SendQueueItem item = sendQueue.get(j);
            if(item.e == EVENT_NEW_MACHINE) {
                choices.add(SchedulerChoice(machine, j, -1));
                doLoop = false;
            } else {
				int stateIdx = item.target->canServeEvent(item.e);
                if (stateIdx >= 0) {
                    choices.add(SchedulerChoice(machine, j, stateIdx));
                    doLoop = false;
                }
            }
        }
        // Machine is state that can serve null event?
		int nullStateIdx = machine->canServeEvent(EVENT_NULL);
        if(nullStateIdx >= 0) {
            choices.add(SchedulerChoice(machine, -1, nullStateIdx));
        }
    }
    if(choices.size() == 0) {
    	return SchedulerChoice(NULL, -1, -1);
    }
    // Choose one and remove from send queue
    int idx = engine.randomInt(choices.size());
    return choices.get(idx);
}

inline void 
RandomScheduler::startMachine(PMachine* machine, const PAny& payload) {
	machines.add(machine);
    machine->start(payload);
}

};

#endif