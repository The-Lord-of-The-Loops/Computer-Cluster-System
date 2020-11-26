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
        //Read Events list and add processes
        Master.ReadEvents(cycles);
        //Read Processes and execute them
        Master.ExecuteProcesses(cycles,ToBeExecuted);
        //Print All info and where Draw UI will be.
        Master.PrintInfo();
        //Check stop condition
        Master.isEmpty(Running);
	}
};