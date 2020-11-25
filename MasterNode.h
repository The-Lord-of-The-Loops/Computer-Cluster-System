#pragma once
#include "Machine.h"
#include "Process.h"
#include "ArrayStack.h"
#include "LinkedQueue.h"
#include "Node.h"
#include "LinkedList.h"
#include "PriorityQueue.h"

class MasterNode
{

public:
	
	/*MasterNode(LinkedList<Machine> * MachineList,	LinkedList<Process> * ProcessList);
	bool Assign(Process Process);
	bool deleteProcess(Process Process);
	bool Wait(Process Process);
	bool Promote(Process Process);
	void AddtoWaitlist(Process Process);
	~MasterNode();*/

	LinkedList<Machine>* MachineList;
	//LinkedList<Process>* ProcessList;
	PriorityQueue<Process> SysWaitingList;
	LinkedQueue<Process> InterWaitingList;
	LinkedQueue<Process> CompIntenWaitingList;
	LinkedList<Process> CompletedProcesses;
};
