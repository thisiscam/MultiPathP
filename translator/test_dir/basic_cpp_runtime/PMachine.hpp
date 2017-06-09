#ifndef PMACHINE_HPP
#define PMACHINE_HPP

#include <vector>
#include <stdexcept>
#include <string>

#include "PList.hpp"
#include "PAny.hpp"
#include "PTuple.hpp"
#include "Scheduler.hpp"

using namespace std;

namespace basic_cpp_runtime {

const int EXECUTE_FINISHED = 0;
const int RAISED_EVENT = 1;

const int EVENT_NEW_MACHINE = -1;
const int EVENT_NULL = 0;

class PMachine
{
public:
	typedef void (PMachine::*TransitionFunction)(const PAny& any);
	typedef void (PMachine::*ExitFunction)();

	virtual void start(const PAny& payload) = 0;

	int canServeEvent(int e) {
		for(int i=states.size-1; i >= 0; i--) {
            int state = states.get(i);
            if(isDefered(state, e)) {
                return -1;
            } else if(getTransition(state, e) != NULL) {
                return i;
            }
        }
        throw runtime_error("Unhandled event");
	}

	void step(int stateIndex, int e, const PAny& payload) {
		int state = states.get(stateIndex);
        if(isGotoTransition(state, e)) {
            for(int i=states.size - 1; i > stateIndex; i--)
            {
                popState();
            }
        }
        retcode = EXECUTE_FINISHED;
        TransitionFunction transition_fn = getTransition(state, e);
        (this->*transition_fn)(payload);
	}

protected:
	void send(PMachine* other, int e, const PAny& payload) {
		// sendQueue.add(PTuple<PMachine*, int, PAny>(other, e, payload));
	}

	template<typename M>
	void create(const PAny& payload) {
		// sendQueue.add(PTuple<PMachine*, int, PAny>(NULL, EVENT_NEW_MACHINE, payload));
	}

	void raiseEvent(int e, const PAny& payload) {
		for(int i=states.size-1; i >= 0; i--) {
            int state = states.get(i);
            TransitionFunction f = getTransition(state, e);
            if(f != NULL) {
                (this->*f)(payload);
                return;
            } else {
                popState();
            }
        }
        throw runtime_error(string("Unhandled event"));
	}

	void popState() {
        int last = states.size - 1;
        int current_state = states.get(last);
        states.removeRange(last);
        ExitFunction eF = getExitFunction(current_state);
        if(eF != NULL) {
            (this->*eF)();
        }
    }

    bool randomBool() {
    	return true;
    }

    void assert(bool cond, const string& message) {
    	if(!cond) {
    		throw runtime_error(message);
    	}
    }


	virtual bool isDefered(int state, int event) const = 0;
	virtual bool isGotoTransition(int state, int event) const = 0;
	virtual ExitFunction getExitFunction(int state) const = 0;
	virtual TransitionFunction getTransition(int state, int event) const = 0;

	int retcode;
	PList<int> states;

private:
	PList<PTuple<PMachine*, int, PAny>> sendQueue;
};

};

#endif