#pragma once
#include "Machine.h"
#include "Process.h"
#include "ArrayStack.h"
#include "LinkedQueue.h"
#include "Node.h"
#include "LinkedList.h"
class MasterNode
{
public:
	
	MasterNode(LinkedList<Machine> MachineList, LinkedList<Process> ProcessList);
	bool Assign(Process Process);

	~MasterNode();

private:

};
