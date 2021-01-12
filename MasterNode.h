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
	int BGP, BGU, BIO;			// reboot duration for each type in cycles
	int AutoP;					//number of cycles after which an interactive process is promoted to a system process
	int E;						// total number of events
	int prom = 0;				// NO. of auto promoted processes

	PriorityQueue<Machine> AV_GP_Machines; //array Available of GP Machines
	PriorityQueue<Machine> AV_GU_Machines; //array Available of GU Machines
	PriorityQueue<Machine> AV_IO_Machines; //array Available of IO Machines

	PriorityQueue<Machine> EX_GP_Machines; //array Working (in execution) of GP Machines
	PriorityQueue<Machine> EX_GU_Machines; //array Working (in execution) of GU Machines
	PriorityQueue<Machine> EX_IO_Machines; //array Working (in execution) of IO Machines

	PriorityQueue<Machine> Rbooting_Machines; // arry of rebooting machines


	LinkedQueue<Event *> queEvents; // array of events pointers
	Event **ev;
	LinkedList<Process> SysInExecution;		// linked list of processes in execution
	LinkedList<Process> InterInExecution;	// linked list of processes in execution
	LinkedList<Process> CompInExecution;	// linked list of processes in execution
	LinkedList<Process> CompletedProcesses; // linked list of completed process

public:
	MasterNode();
	void ReadNecessaryData(string infile); // Reads number of machines of each type and reboot duration ect..
	void PrintInfo();

	// Printing Functios
	void PrintAvMacIDs();
	void Printno_Av_Machines();
	void Printno_Wa_Process();
	void PrintWaProcIDs();
	void Printno_In_Execution();
	void PrintInExecIDs();
	void Printno_Completed(int NoInteractive, int NoCompInt, int NoSys);
	void PrintCompletedIDs(int &NoInteractive, int &NoCompInt, int &NoSys);

	//Excutes an events when the clock equals its arrival time.
	void ExecuteEvents(bool &exev);

	//calls CompleteProcess for every process in every InExcution list.
	void complete();

	//checkes if the process is done at the current clock cycle and moves it to completed processes list
	bool completeProcess(Process &process);

	// returns machine to be available after its assigned process is done.
	bool completeMachine(int ID, MachineType type);


	// calls the next function for every process in the waiting lists.
	void dispatch();

	// change the process status to " Dispatched" if its time has come and calls the Assign for this process.
	//Or increaments its waiting time.
	bool DispatchProcess(Process &process);

	// assign process according to the required assignment criteria
	bool Assign(Process &process);

	// checks if there is any excuting processes in the excution lists.
	bool IsExecuting();

	// Auto promote interactive processes.
	void AutoPromte();

	~MasterNode();

	//Phase 2

	// the main function in the program.
	void Simulate(const string path);
	
	//
	void Analyze(bool &exev);

	void SaveToFile(const string inputfile);


	bool Check(bool &exev);
	
	//prints the process assined last cycle.
	void FindAssignedLastCycle();
	
	
	bool SilentCheck();
	
	// Moves machines back to be available after is has completed its rebooting time.
	void ReturnFromReboot();

	LinkedList<Process> SysWaitingList;

	LinkedList<Process> InterWaitingList;

	LinkedQueue<Process> CompIntenWaitingList;
};
