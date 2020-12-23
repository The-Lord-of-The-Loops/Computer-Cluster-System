#include "MasterNode.h"
#include "Event.h"

int main(){

	MasterNode master;
	cout << "Cycle: " << 0 << endl;
	master.PrintInfo();
	//master.SimpleSimulation("test2.txt");
	master.Simulate();
	return 0;
};