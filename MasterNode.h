#pragma once
#include "Machine.h"
#include "Process.h"
#include "ArrayStack.h"
#include "LinkedQueue.h"
#include "Node.h"
#include "LinkedList.h"
#include "PriorityQueue.h"
#include "Event.h"
#include "ArrivalEvent.h"
#include "CancelEvent.h"
#include "PromotEvent.h"

class MasterNode{
public:
	MasterNode(LinkedList<Machine> * MachineList,	LinkedList<Process> * ProcessList, LinkedList<Event> *Events);
	void ReadEvents(int cycle);
	void ExecuteProcesses(int cycle, LinkedList<Process>* ToBeExecuted);
	void PrintInfo();
	bool Assign(Process Process);
	bool deleteProcess(Process Process);
	bool Promote(Process Process);
	void AddtoWaitlist(Process Process);
	void isEmpty(bool &Running);
	~MasterNode();

	LinkedList<Event>* Events;
	LinkedList<Machine>* MachineList;
	LinkedList<Process>* ProcessList;
	PriorityQueue<Process> SysWaitingList;
	LinkedQueue<Process> InterWaitingList;
	LinkedQueue<Process> CompIntenWaitingList;
	LinkedList<Process> CompletedProcesses;
};
