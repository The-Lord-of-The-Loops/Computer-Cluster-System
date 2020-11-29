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

using namespace std;

class MasterNode
{
private:
	unsigned long int clock;	//  the clock
	int no_GP, no_GU, no_IO;	// number of machines for each type
	int rsp_GP, rsp_GU, rsp_IO; // response time for each type
	int N;						// number of processes before rebooting
	unsigned int BGP, BGU, BIO; // reboot duration for each type in cycles
	unsigned int AutoP;			//number of cycles after which an interactive process is promoted to a system process
	unsigned int E;				// total number of events

	Machine **GP_Machines; //array of GP Machines
	Machine **GU_Machines; //array of GU Machines
	Machine **IO_Machines; //array of IO Machines

	Event **arrEvents; // array of events pointers
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


	bool Assign(Process Process);
	bool deleteProcess(Process Process);
	bool Promote(Process Process);
	void Operate();
	~MasterNode();

	PriorityQueue<Process> SysWaitingList;
	LinkedQueue<Process> InterWaitingList;
	LinkedQueue<Process> CompIntenWaitingList;
};
