type ProposalIdType = (serverid: int, round: int);
type ProposalType = (pid: ProposalIdType, value: int);


// Events related to Paxos
event prepare assume 3: (proposer: machine, proposal: ProposalType);
event accept assume 3: (proposer: machine, proposal: ProposalType);
event agree assume 3: ProposalType;
event reject assume 3: ProposalIdType;
event accepted assume 3: ProposalType;


//Global constants
event GC_NumOfAccptNodes;
event GC_NumOfProposerNodes;
event GC_Default_Value;
