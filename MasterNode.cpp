#include "MasterNode.h"

MasterNode::MasterNode(string inputfile, string outputfile)
{
	ReadNecessaryData(inputfile);
	clock = 0;
}

bool MasterNode::Assign(Process process)
{
	if (process.GetProcessType() == System)
	{
		for (int i = 1; i < no_GP+1; i++)
		{
			if (GP_Machines[i]->isAvailable())
			{
				GP_Machines[i]->setAvailability(false);
				InExecution.InsertLast(process);
				return true;
			}
		}

		for (int i = 1; i < no_IO+1; i++)
		{
			if (IO_Machines[i]->isAvailable())
			{
				IO_Machines[i]->setAvailability(false);
				InExecution.InsertLast(process);
				return true;
			}
		}

		for (int i = 1; i < no_GU+1; i++)
		{
			if (GU_Machines[i]->isAvailable())
			{
				GU_Machines[i]->setAvailability(false);
				InExecution.InsertLast(process);
				return true;
			}
		}
		return false;
	}
	else if (process.GetProcessType() == interactive)
	{

		for (int i = 1; i < no_IO+1; i++)
		{
			if (IO_Machines[i]->isAvailable())
			{
				IO_Machines[i]->setAvailability(false);
				InExecution.InsertLast(process);
				return true;
			}
		}

		for (int i = 1; i < no_GP+1; i++)
		{
			if (GP_Machines[i]->isAvailable())
			{
				GP_Machines[i]->setAvailability(false);
				InExecution.InsertLast(process);
				return true;
			}
		}
		return false;
	}
	else if (process.GetProcessType() == ComputationallyIntensive)
	{
		for (int i = 1; i < no_GU+1; i++)
		{
			if (GU_Machines[i]->isAvailable())
			{
				GU_Machines[i]->setAvailability(false);
				InExecution.InsertLast(process);
				return true;
			}
		}
		return false;
	}
}

bool MasterNode::deleteProcess(Process Process)
{
	return false;
}

bool MasterNode::Promote(Process Process)
{
	return false;
}

void MasterNode::Operate()
{
	
	bool operate = true;

	while (operate)
	{
		clock++;
		//Assigning Events
		for (int i = 1; i < E+1; i++)
		{
			if (arrEvents[i]->ArrivalTime == clock)
				arrEvents[i]->Execute();
		}

		//Executing Processes
		bool Assigned = true;
		Process process;
		while (Assigned)
		{
			if(SysWaitingList.dequeue(process))
				Assigned = Assign(process);
			if (!Assigned)
				SysWaitingList.enqueue(process);
		}
		Assigned = true;
		while (Assigned)
		{
			if(InterWaitingList.dequeue(process))
				Assigned = Assign(process);
			if (!Assigned)
				InterWaitingList.enqueue(process);
		}
		Assigned = true;
		while (Assigned)
		{
			if(CompIntenWaitingList.dequeue(process))
				Assigned = Assign(process);
			if (!Assigned)
				CompIntenWaitingList.enqueue(process);
		}

		//Checking for operations
		if (!arrEvents[E] && SysWaitingList.isEmpty() && InterWaitingList.isEmpty() && CompIntenWaitingList.isEmpty() && InExecution.isEmpty())
		{
			operate = false;
		}
	}
}


void MasterNode::ReadNecessaryData(string infile)
{
	ifstream Infile;
	Infile.open(infile);
	Infile.open(infile);
	Infile >> no_GP >> no_GU >> no_IO >> rsp_GP >> rsp_GU >> rsp_IO >> N >> BGP >> BGU >> BIO >> AutoP >> E;
	GP_Machines = new Machine * [no_GP];
	GU_Machines = new Machine * [no_GU];
	IO_Machines = new Machine * [no_IO];
	//cout << " " << no_GP << " " << no_GU << " " << no_IO << " " << rsp_GP << " " << rsp_GU << " " << rsp_IO << " " << N << " " << BGP << " " << BGU << " " << BIO << " " << AutoP << " " << E;
	//creating Machines
	for (int i = 1; i < no_GP + 1; i++)
	{
		GP_Machines[i] = new Machine(i,rsp_GP, N, BGP);
	}
	for (int i = 1; i < no_GU + 1; i++)
	{
		GU_Machines[i] = new Machine(i,rsp_GU, N, BGU);
	}
	for (int i = 1; i < no_GP + 1; i++)
	{
		GP_Machines[i] = new Machine(i,rsp_IO, N, BIO);
	}
	char EventType, processtype;
	int at, id, dl, et, p;

	for (int i = 1; i < E+1; i++)
	{
		Infile >> EventType;

		if (EventType == 'A')
		{
			Infile >> processtype >> at >> id >> dl >> et >> p;
			arrEvents[i] = new ArrivalEvent(at, id, (processtype == 'S') ? System : (processtype == 'I') ? Interactive : ComputationallyIntensive, dl, et, p, this); ////check this
		
		}
		if (EventType == 'X')
		{
			Infile  >> at >> id;
			arrEvents[i] = new CancelEvent(at, id, this); //////////**
		
		}
		if (EventType == 'P')
		{
			Infile  >> at >> id;
			arrEvents[i] = new CancelEvent(at, id, this);
		
		}
	}

	Infile.close();
}

MasterNode::~MasterNode() = default;
