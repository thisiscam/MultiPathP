#ifndef PMACHINE_HPP
#define PMACHINE_HPP

#include <memory>
#include <vector>
#include <stdexcept>

#include "IPType.hpp"
#include "List.hpp"

using namespace std;

namespace basic_cpp_runtime {

const int EXECUTE_FINISHED = 0;
const int RAISED_EVENT = 1;

const int EVENT_NEW_MACHINE = -1;
const int EVENT_NULL = 0;

class PMachineID;
using PMachineIDPtr = unique_ptr<PMachineID>;

class PMachine
{
public:

protected:
	// void send(PMachineIDPtr other, PBoxedIntPtr e, )
	int retcode;
	ListPtr<int> states = List<int>::newEmpty();

	// Scheduler* scheduler;

	// const bool deferedSet[][];
	// const bool isGotoTransitions[][];

private:

};

class PMachineID final : public IPType
{
private:
	struct PMachineIDKind {
		const char* tag = "PMachineIDKind";
	};
	static const PMachineIDKind _kind;

public:
	static constexpr const PMachineIDKind* kind() {
		return &_kind;
	}

	PMachineID(PMachine* machine):machine(machine) {}

	static PMachineIDPtr create(PMachine* machine) {
		return PMachineIDPtr(new PMachineID(machine));
	}

	IPTypePtr dynamicCopy() override {
		return IPTypePtr(new PMachineID(machine));
	}

	PMachineIDPtr copy() {
		return PMachineIDPtr(new PMachineID(machine));
	}
	
	bool equals(const IPType *other) const override {
		const PMachineID* o = dynamic_cast<const PMachineID*>(other);
		return o != NULL && machine == o->machine;
	}

	void checkType(const IPType::Kind* targetKind) override {
		if(targetKind != (IPType::Kind*)kind() && targetKind != IPType::kind()) {
			throw bad_cast();
		}
	}

	PMachine* const machine;
};

};

#endif