#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

namespace basic_cpp_runtime {

class Scheduler {

protected:

	struct SchedulerChoice
    {
        PMachine* machine;
        int queueIdx;
        int stateIdx;
    };

	virtual bool chooseMachine(SchedulerChoice& choice) = 0;

	virtual void startMachine(PMachine* machine, const PAny& payload) = 0;

	inline PList<SendQueueItem> getSendQueue(PMachine* machine) {
		return machine->sendQueue;
	}

	PList<PMachine*> machines;
	
public:
	inline bool step() {
		SchedulerChoice&& chosen = chooseMachine(chosen);
		if(chosen.machine == NULL) {
			return false;
		} else {
			if(chosen.queueIdx < 0) {
				std::cout << chosen.machine << " executes null event" << std::endl;
				chosen.machine->step(chosen.stateIdx, EVENT_NULL);
			} else {
				SendQueueItem&& item = chosen.machine.sendQueue.get(chosen.queueIdx);
				chosen.machine.sendQueue.removeAt(chosen.queueIdx);
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

};

};

#endif