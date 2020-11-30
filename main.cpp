#include "MasterNode.h"
#include "Event.h"

int main(){

	MasterNode master;
	cout << "Cycle: " << 0 << endl;
	master.PrintInfo();
	master.SimpleSimulation("test3.txt");
	return 0;
};