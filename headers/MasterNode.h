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
	unsigned long int clock; //  the clock 
	int no_GP, no_GU, no_IO; // number of machines for each type
	int rsp_GP, rsp_GU, rsp_IO; // response time for each type
	int N; // number of processes before rebooting
	unsigned int BGP, BGU, BIO; // reboot duration for each type in cycles
	unsigned int AutoP; //number of cycles after which an interactive process is promoted to a system process 
	unsigned int E; // total number of event s

	 Machine* GP_Machines= new Machine [no_GP]; //array of GP Machines
	 Machine* GU_Machines= new Machine [no_GU]; //array of GU Machines
	 Machine* IO_Machines= new Machine [no_IO]; //array of IO Machines

public:
	MasterNode(string inputfile, string outputfile);
	void ReadNecessaryData(string infile); // Reads number of machines of each type and reboot duration ect.. 
	void WriteResults(string outfile);
	void ExecuteProcesses(int cycle, LinkedList<Process> *ToBeExecuted);
	void PrintInfo();
	bool Assign(Process Process);
	bool deleteProcess(Process Process);
	bool Promote(Process Process);
	void AddtoWaitlist(Process Process);
	void isEmpty(bool &Running);
	~MasterNode();

	LinkedList<Event> *Events;
	LinkedList<Machine> *MachineList;
	LinkedList<Process> *ProcessList;
	PriorityQueue<Process> SysWaitingList;
	LinkedQueue<Process> InterWaitingList;
	LinkedQueue<Process> CompIntenWaitingList;
	LinkedList<Process> CompletedProcesses;
};
