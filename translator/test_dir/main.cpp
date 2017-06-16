#include "header.h"

using namespace pingpong;

int main(int argc, char const *argv[])
{
	Scheduler scheduler;
	ExecutionEngine engine = ExecutionEngine(scheduler);
	engine.run(Controller::createMainMachine(engine));
	return 0;
}