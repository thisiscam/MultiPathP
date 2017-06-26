/* Dummy Two Phase Commit Protocol */

event eUnit;
event eTransaction: int;
event eCommit;
event eSuccess;
event eFailure;

main machine Main {
	start state Init {
		entry {
			var coor: machine;
			var index: int;
			coor = new CoordinateMachine();
			if($) {
				send coor, eTransaction;
			}			
			if($) {
				send coor, eTransaction;
			}			
			if($) {
				send coor, eTransaction;
			}
		}
	}
}

machine CoordinateMachine {
	var participant: machine;

	start state Init {
		entry {
			participant = new ParticipantMachine(this);
			raise eUnit;
		}
		on eUnit goto WaitForRequest;
	}

	state WaitForRequest {
		on eTransaction goto TransactionState;
	}

	state TransactionState {
		defer eTransaction;
		entry {
			send participant, eCommit;
		}

		on eSuccess, eFailure goto WaitForRequest;
	}
}

machine ParticipantMachine {
	var coor: machine;
	start state Init {
		entry (payload: machine) {
			coor = payload;
			raise eUnit;
		}
		on eUnit goto WaitForRequest;
	}

	state WaitForRequest {
		on eCommit do {
			if($)
			{
				send coor, eSuccess;
			}
			else
			{
				send coor, eFailure;
			}
		}
	}

}