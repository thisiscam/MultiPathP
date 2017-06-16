#ifndef RANDOM_SCHEDULER
#define RANDOM_SCHEDULER

namespace basic_cpp_runtime {

class RandomScheduler final : public Scheduler {

protected:

	inline SchedulerChoice chooseMachine() override {
		PList<SchedulerChoice> choices;
		for(int i=0; i < this.machines.size(); i++) {
	        PMachine* machine = machines.get(i);
	        // Collect from send queue
	        PList<SendQueueItem> sendQueue = getSendQueue(machine);
	        bool doLoop = true;
			for(int j=0; doLoop && j < sendQueue.size(); j++) {
	            SendQueueItem&& item = sendQueue.get(j);
	            if(item.e == EVENT_NEW_MACHINE) {
	                choices.add(SchedulerChoice(machine, j, -1));
	                doLoop = false;
	            } else {
					int stateIdx = item.target.canServeEvent(item.e);
	                if (stateIdx >= 0) {
	                    choices.Add(SchedulerChoice(machine, j, stateIdx));
	                    doLoop = false;
	                }
	            }
	        }
	        // Machine is state that can serve null event?
			int nullStateIdx = machine.canServeEvent(EVENT_NULL);
	        if(nullStateIdx >= 0) {
	            choices.add(SchedulerChoice(machine, -1, nullStateIdx));
	        }
	    }
	    if(choices.size() == 0) {
	    	return null;
	    }
        // Choose one and remove from send queue
        int idx = engine.randomInt();
        return choices.get(idx);
	}

	inline void startMachine(PMachine* machine, const PAny& payload) override {
		machines.add(machine);
        machine.start(engine, payload);
	}

};

};

#endif