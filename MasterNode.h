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
#include <fstream>

class MasterNode
{
private:
	unsigned long int cycles;	//  the clock
	int no_GP, no_GU, no_IO;	// number of machines for each type
	int rsp_GP, rsp_GU, rsp_IO; // response time for each type
	int N;						// number of processes before rebooting
	int BGP, BGU, BIO; // reboot duration for each type in cycles
	int AutoP;			//number of cycles after which an interactive process is promoted to a system process
	int E;				// total number of events

	Machine *GP_Machines = new Machine[no_GP]; //array of GP Machines
	Machine *GU_Machines = new Machine[no_GU]; //array of GU Machines
	Machine *IO_Machines = new Machine[no_IO]; //array of IO Machines

	Event **arrEvents; // array of events pointers
    int TotalNumberOfEvents;

    LinkedList<Process> InExecution; // linked list of processes in execusion
	LinkedList<Process> CompletedProcesses; // linked list of completed process

public:
	MasterNode(string inputfile, string outputfile);
	MasterNode();
	void ReadNecessaryData(string infile); // Reads number of machines of each type and reboot duration ect..

	void WriteResults(string outfile);
	void ExecuteProcesses(Process process);
	void PrintInfo();

	//Phase 1
	void SimpleSimulation(string inputfile);
	void PrintAvMacIDs();
	void Printno_Av_Machines();
	void Printno_Wa_Process();
	void PrintWaProcIDs();
	void Printno_In_Execution();
	void PrintInExecIDs();
	void ExecuteEvents(bool &exev);
	void ExecuteOneProcessOfEachType();

	bool Assign(Process Process);
	bool deleteProcess(Process Process);
	bool Promote(Process Process);
	void Operate();
	~MasterNode();

	LinkedList<Process> SysWaitingList;
	//To Sorted linked list

	LinkedQueue<Process> InterWaitingList;
	LinkedQueue<Process> CompIntenWaitingList;
};
