#ifndef EXECUTION_ENGINE_HPP
#define EXECUTION_ENGINE_HPP

namespace basic_cpp_runtime {

class ExecutionEngine {
public:
	ExecutionEngine(Scheduler& scheduler, int maxIteration=200):
		scheduler(scheduler),
		maxIteration(maxIteration)
	{ }

	inline void run(PMachine* machine) {
		scheduler.startMachine(machine);
		for(int i = 0; i < maxIteration; i++) {
			if(!scheduler.step()) {
				break;
			}
		}
	}

private:
	Scheduler& scheduler;
	int maxIteration;
};

};

#endif