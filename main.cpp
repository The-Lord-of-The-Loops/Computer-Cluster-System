#include "MasterNode.h"

int main() {

	int cycles = 0;
	bool Running = true;
	LinkedList<Machine>* MachineList = new LinkedList<Machine>;
	LinkedList<Process>* ProcessList = new LinkedList<Process>;
	MasterNode Master(MachineList, ProcessList);

	while (Running)
	{

	}
};