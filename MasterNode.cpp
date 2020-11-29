#include "MasterNode.h"

MasterNode::MasterNode(string inputfile, string outputfile)
{
	ReadNecessaryData(inputfile);
	clock = 0;
}

MasterNode::MasterNode()
{
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

void MasterNode::SimpleSimulation(string inputfile)
{
	ReadNecessaryData(inputfile);
	bool operate = true;
	bool nextcycle = true;
	while (operate & nextcycle)
	{
		clock++;

		//Printing to consol
		Printno_Av_Machines();
		PrintAvMacIDs();

		Printno_Wa_Process();
		PrintWaProcIDs();

		Printno_In_Execution();
		PrintInExecIDs();

		//Assigning Events
		for (int i = 1; i < E + 1; i++)
		{
			if (arrEvents[i]->ArrivalTime == clock)
				arrEvents[i]->Execute(SysWaitingList, InterWaitingList, CompIntenWaitingList);
		}
		Process process;
		bool dequeued;

		dequeued = SysWaitingList.dequeue(process);
		if (dequeued)
			InExecution.InsertLast(process);

		dequeued = InterWaitingList.dequeue(process);
		if (dequeued)
			InExecution.InsertLast(process);

		dequeued = CompIntenWaitingList.dequeue(process);
		if (dequeued)
			InExecution.InsertLast(process);
		
		Node<Process>* p = InExecution.Head;
		bool Syscomp = false, Intercomp = false, compcomp = false;
		if (clock % 5 == 0)
		{
			while (p)
			{
				process = p->getItem();
				if (process.GetProcessType()== System && !Syscomp)
				{
					CompletedProcesses.InsertLast(process);
					Syscomp = true;
				}
				else if (process.GetProcessType() == Interactive && !Intercomp)
				{
					CompletedProcesses.InsertLast(process);
					Intercomp = true;
				}
				else if (process.GetProcessType() == ComputationallyIntensive && !compcomp)
				{
					CompletedProcesses.InsertLast(process);
					compcomp = true;
				}
				if (Syscomp && Intercomp && compcomp)
					break;
				p = p->getNext();
			}
		}

		cin >> nextcycle;
		//Checking for operations
		if (!arrEvents[E] && SysWaitingList.isEmpty() && InterWaitingList.isEmpty() && CompIntenWaitingList.isEmpty() && InExecution.isEmpty())
		{
			operate = false;
		}
	}
}

void MasterNode::PrintAvMacIDs()
{
	cout << "Available Machines: ";
	for (int i = 1; i < no_GP+1; i++)
	{
		if (GP_Machines[i]->isAvailable())
			cout << "[ " << GP_Machines[i]->getID() << " ]" << "   ";
	}
	for (int i = 1; i < no_GU+1; i++)
	{
		if (GU_Machines[i]->isAvailable())
			cout << "( " << GU_Machines[i]->getID() << " )" << "   ";
	}
	for (int i = 1; i < no_IO+1; i++)
	{
		if (IO_Machines[i]->isAvailable())
			cout << IO_Machines[i]->getID() << "   ";
	}
	cout << endl;
}

void MasterNode::Printno_Av_Machines()
{
	int ngp = 0, ngu = 0, nio = 0;
	cout << "Printing No. of Available Machines:" << endl;
	for (int i = 1; i < no_GP+1; i++)
	{
		if (GP_Machines[i]->isAvailable())
			ngp++;
	}
	cout << "[ " << ngp << " ]" << "   ";
	for (int i = 1; i < no_GU+1; i++)
	{
		if (GU_Machines[i]->isAvailable())
			ngu++;
	}
	cout << "( " << ngu << " )" << "   ";
	for (int i = 1; i < no_IO+1; i++)
	{
		if (IO_Machines[i]->isAvailable())
			nio++;
	}
	cout << nio << endl;
	cout << " ------------------------------- " << endl;
}

void MasterNode::Printno_Wa_Process()
{
	cout << "Printing No. of Waiting Processes:" << endl;
	cout << "[ " << SysWaitingList.GetCount() << " ]" << "   ";
	cout << "( " << InterWaitingList.GetCount() << " )" << "   ";
	cout << CompIntenWaitingList.GetCount() << endl;
}

void MasterNode::PrintWaProcIDs()
{
	Process process;
	cout << "Waiting Processes: ";
	for (int i = 0; i < SysWaitingList.GetCount(); i++)
	{
		SysWaitingList.dequeue(process);
		cout << "[ " << process.GetID() << " ]" << "  ";
		SysWaitingList.enqueue(process,process.GetPriority());
	}
	for (int i = 0; i < InterWaitingList.GetCount(); i++)
	{
		InterWaitingList.dequeue(process);
		cout << "( " << process.GetID() << ")" << "  ";
		InterWaitingList.enqueue(process);
	}
	for (int i = 0; i < CompIntenWaitingList.GetCount(); i++)
	{
		CompIntenWaitingList.dequeue(process);
		cout << process.GetID() << "  ";
		CompIntenWaitingList.enqueue(process);
	}
	cout << endl;
}

void MasterNode::Printno_In_Execution()
{
	int sysp =0 , interp = 0, compp = 0;
	cout << "Printing No. of In Execution Processes: " << endl;
	Node<Process>* p = InExecution.Head;
	Process process;
	while (p)
	{
		process = p->getItem();
		if (process.GetProcessType() == System)
			sysp++;
		else if (process.GetProcessType() == Interactive)
			interp++;
		else
			compp++;
		p = p->getNext();
	}
	cout << "[ " << sysp << " ]" << "  " << "( " << interp << " )" << "  " << compp << endl;
}

void MasterNode::PrintInExecIDs()
{
	cout << "In Execution Processes: ";
	Node<Process>* p = InExecution.Head;
	Process process;
	while (p)
	{
		process = p->getItem();
		if (process.GetProcessType() == System)
			cout << "[ " << process.GetID() << " ]" << "  ";
		else if (process.GetProcessType() == Interactive)
			cout << "( " << process.GetID() << ")" << "  ";
		else
			cout << process.GetID() << endl;
		p = p->getNext();
	}
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
				arrEvents[i]->Execute(SysWaitingList,InterWaitingList, CompIntenWaitingList);
		}

		//Executing Processes
		bool Assigned = true;
		Process process;
		while (Assigned)
		{
			if(SysWaitingList.dequeue(process))
				Assigned = Assign(process);
			if (!Assigned)
				SysWaitingList.enqueue(process,process.GetPriority());
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
	arrEvents = new Event * [E];
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
			arrEvents[i] = new ArrivalEvent(at, id, (processtype == 'S') ? System : (processtype == 'I') ? Interactive : ComputationallyIntensive, dl, et, p);
		
		}
		if (EventType == 'X')
		{
			Infile  >> at >> id;
			arrEvents[i] = new CancelEvent(at, id);
		
		}
		if (EventType == 'P')
		{
			Infile  >> at >> id;
			arrEvents[i] = new CancelEvent(at, id);
		
		}
	}

	Infile.close();
}

MasterNode::~MasterNode() = default;
