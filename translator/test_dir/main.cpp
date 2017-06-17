#include "header.h"

using namespace pingpong;

int main(int argc, char const *argv[])
{
	ExecutionEngine engine = ExecutionEngine();
	RandomScheduler scheduler(engine);
	PMachine* mainMachine = Controller::createMainMachine(engine);
	engine.run(scheduler, mainMachine);
	return 0;
}