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
#include "UI.h"
using namespace std;

class MasterNode
{
private:
    PROG_MODE Mode;
	unsigned long int clock;	//  the clock
	int no_GP, no_GU, no_IO;	// number of machines for each type
	int rsp_GP, rsp_GU, rsp_IO; // response time for each type
	int N;						// number of processes before rebooting
	int BGP, BGU, BIO; // reboot duration for each type in cycles
	int AutoP;			//number of cycles after which an interactive process is promoted to a system process
	int E;				// total number of events
	int prom = 0;

	PriorityQueue<Machine> AV_GP_Machines; //array of GP Machines
	PriorityQueue<Machine> AV_GU_Machines; //array of GU Machines
	PriorityQueue<Machine> AV_IO_Machines; //array of IO Machines

	PriorityQueue<Machine> EX_GP_Machines;
	PriorityQueue<Machine> EX_GU_Machines;
	PriorityQueue<Machine> EX_IO_Machines;

	LinkedQueue<Event*> queEvents; // array of events pointers
	Event** ev;
    LinkedList<Process> SysInExecution; // linked list of processes in execution
	LinkedList<Process> InterInExecution; // linked list of processes in execution
	LinkedList<Process> CompInExecution; // linked list of processes in execution
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
	void Printno_Completed(int NoInteractive, int NoCompInt, int NoSys);
    void PrintCompletedIDs(int &NoInteractive, int &NoCompInt, int &NoSys);
	void ExecuteEvents(bool &exev);

	//Underconstruction
	void complete();
	bool complete(Process &process);
	bool complete(int ID, MachineType type);
	void dispatch();
	bool dispatch(Process &process);
	void execute();
	bool execute(Process process);
	bool Assign(Process &process);
	bool IsExecuting();
	void Checkup();
	void AutoPromte();
	~MasterNode();

	//Phase 2
	void Simulate(const string path);
	void Analyze(bool &exev);
	void SaveToFile(const string inputfile);
	bool Check(bool& exev);
	void FindAssignedLastCycle();
	bool SilentCheck();


	LinkedList<Process> SysWaitingList;
	//To Sorted linked list

	LinkedList<Process> InterWaitingList;

	LinkedQueue<Process> CompIntenWaitingList;



};
