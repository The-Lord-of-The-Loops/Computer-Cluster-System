#include "MasterNode.h"
#include "Event.h"

int main() {

	int cycles = 0;
	bool Running = true;
	LinkedList<Event>* Events = new LinkedList<Event>;
	LinkedList<Machine>* MachineList = new LinkedList<Machine>;
	LinkedList<Process>* ProcessList = new LinkedList<Process>;
	LinkedList<Process>* ToBeExecuted = new LinkedList<Process>;

	MasterNode Master(MachineList, ProcessList, Events);

	//Read the files here.

	//^^^^^^^^^^^^^^^^^^^
	while (Running)
	{
	    cycles++;
	}
};