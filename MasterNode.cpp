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
		for (int i = 0; i < no_GP; i++)
		{
			if (GP_Machines[i].isAvailable())
			{
				GP_Machines[i].setAvailability(false);
				InExecution.InsertLast(process);
				return true;
			}
		}

		for (int i = 0; i < no_IO; i++)
		{
			if (IO_Machines[i].isAvailable())
			{
				IO_Machines[i].setAvailability(false);
				InExecution.InsertLast(process);
				return true;
			}
		}

		for (int i = 0; i < no_GU; i++)
		{
			if (GU_Machines[i].isAvailable())
			{
				GU_Machines[i].setAvailability(false);
				InExecution.InsertLast(process);
				return true;
			}
		}
		return false;
	}
	else if (process.GetProcessType() == Interactive)
	{

		for (int i = 0; i < no_IO; i++)
		{
			if (IO_Machines[i].isAvailable())
			{
				IO_Machines[i].setAvailability(false);
				InExecution.InsertLast(process);
				return true;
			}
		}

		for (int i = 0; i < no_GP; i++)
		{
			if (GP_Machines[i].isAvailable())
			{
				GP_Machines[i].setAvailability(false);
				InExecution.InsertLast(process);
				return true;
			}
		}
		return false;
	}
	else if (process.GetProcessType() == ComputationallyIntensive)
	{
		for (int i = 0; i < no_GU; i++)
		{
			if (GU_Machines[i].isAvailable())
			{
				GU_Machines[i].setAvailability(false);
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
	bool exev;
	Node<Process>* p;
	Node<Process>* R;
	bool Syscomp, Intercomp, compcomp;
	Process process;
	bool dequeued;
	while (operate && nextcycle)
	{
		cout << "Cycle: " << clock << endl;
		clock++;
		exev = false;
		//Printing to consol
		Printno_Av_Machines();
		PrintAvMacIDs();

		Printno_Wa_Process();
		PrintWaProcIDs();

		Printno_In_Execution();
		PrintInExecIDs();

		//Assigning Events
		for (int i = 0; i < E; i++)
		{
			if (arrEvents[i])
			{
				if (arrEvents[i]->ArrivalTime == clock)
				{
					arrEvents[i]->Execute(SysWaitingList, InterWaitingList, CompIntenWaitingList);
					E--;
					for (int n = i; n < E; n++)
					{
						arrEvents[n] = arrEvents[n + 1];
					}
					arrEvents[E] = nullptr;
					i--;
				}
				exev = true;
			}
		}
		
		
		dequeued = SysWaitingList.peek(process);
		if (dequeued)
		{
			SysWaitingList.dequeue(process);
			InExecution.InsertLast(process);
		}

		dequeued = InterWaitingList.peek(process);
		if (dequeued)
		{
			InterWaitingList.dequeue(process);
			InExecution.InsertLast(process);
		}

		dequeued = CompIntenWaitingList.peek(process);
		if (dequeued)
		{
			CompIntenWaitingList.dequeue(process);
			InExecution.InsertLast(process);
		}
		
		p = InExecution.Head;
		R = p;
		if(p)
			R = p->getNext();
		Syscomp = false; Intercomp = false; compcomp = false;
		if (clock % 5 == 0)
		{
			while (p)
			{
				process = p->getItem();
				if (process.GetProcessType()== System && !Syscomp)
				{
					CompletedProcesses.InsertLast(process);
					InExecution.DeleteNode(p->getItem().GetID());
					p = R;
					if(R)
					R = p->getNext();
					Syscomp = true;
				}
				else if (process.GetProcessType() == Interactive && !Intercomp)
				{
					CompletedProcesses.InsertLast(process);
					InExecution.DeleteNode(p->getItem().GetID());
					p = R;
					if(R)
					R = p->getNext();
					Intercomp = true;
				}
				else if (process.GetProcessType() == ComputationallyIntensive && !compcomp)
				{
					CompletedProcesses.InsertLast(process);
					InExecution.DeleteNode(p->getItem().GetID());
					p = R;
					if(R)
					R = p->getNext();
					compcomp = true;
				}
				if (Syscomp && Intercomp && compcomp)
					break;
				if (!Syscomp && !Intercomp && !compcomp)
				{
					p = p->getNext();
					if(R)
					R = R->getNext();
				}
			}
		}

		//cin >> nextcycle;
		//Checking for operations
		if (!exev && SysWaitingList.isEmpty() && InterWaitingList.isEmpty() && CompIntenWaitingList.isEmpty() && InExecution.isEmpty())
		{
			operate = false;
			break;
		}
	}
}

void MasterNode::PrintAvMacIDs()
{
	cout << "Available Machines: ";
	for (int i = 0; i < no_GP; i++)
	{
		if (GP_Machines[i].isAvailable())
			cout << "[ " << GP_Machines[i].getID() << " ]" << "   ";
	}
	for (int i = 0; i < no_GU; i++)
	{
		if (GU_Machines[i].isAvailable())
			cout << "( " << GU_Machines[i].getID() << " )" << "   ";
	}
	for (int i = 0; i < no_IO; i++)
	{
		if (IO_Machines[i].isAvailable())
			cout << IO_Machines[i].getID() << "   ";
	}
	cout << endl;
}

void MasterNode::Printno_Av_Machines()
{
	int ngp = 0, ngu = 0, nio = 0;
	cout << "Printing No. of Available Machines:" << endl;
	for (int i = 0; i < no_GP; i++)
	{
		if (GP_Machines[i].isAvailable())
			ngp++;
	}
	cout << "[ " << ngp << " ]" << "   ";
	for (int i = 0; i < no_GU; i++)
	{
		if (GU_Machines[i].isAvailable())
			ngu++;
	}
	cout << "( " << ngu << " )" << "   ";
	for (int i = 0; i < no_IO; i++)
	{
		if (IO_Machines[i].isAvailable())
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
		cout << "( " << process.GetID() << " )" << "  ";
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
			cout << "( " << process.GetID() << " )" << "  ";
		else
			cout << process.GetID() << "  ";
		p = p->getNext();
	}
	cout << endl;
}

void MasterNode::Operate()
{
	
	bool operate = true;

	while (operate)
	{
		clock++;
		//Assigning Events
		for (int i = 0; i < E; i++)
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
	Infile >> no_GP >> no_GU >> no_IO >> rsp_GP >> rsp_GU >> rsp_IO >> N >> BGP >> BGU >> BIO >> AutoP >> E;
	arrEvents = new Event *[E];
	//cout << " " << no_GP << " " << no_GU << " " << no_IO << " " << rsp_GP << " " << rsp_GU << " " << rsp_IO << " " << N << " " << BGP << " " << BGU << " " << BIO << " " << AutoP << " " << E;
	//creating Machines
	for (int i = 0; i < no_GP; i++)
	{
		Machine GpMach(i, rsp_GP, N, BGP);
		GP_Machines[i] = GpMach;
	}
	for (int i = 0; i < no_GU; i++)
	{
		Machine GuMach(i + no_GP, rsp_GU, N, BGU);
		GU_Machines[i] = GuMach;
	}
	for (int i = 0; i < no_IO; i++)
	{
		Machine IoMach(i + no_GP + no_GU, rsp_IO, N, BIO);
		IO_Machines[i] = IoMach;
	}
	char EventType, processtype;
	int at, id, dl, et, p;

	for (int i = 0; i < E; i++)
	{
		Infile >> EventType;

		if (EventType == 'A')
		{
			Infile >> processtype >> at >> id >> dl >> et >> p;
			arrEvents[i] = new ArrivalEvent(at, id, (processtype == 'S') ? System : (processtype == 'I') ? Interactive : ComputationallyIntensive, dl, et, p);
		
		}
		else if (EventType == 'X')
		{
			Infile  >> at >> id;
			arrEvents[i] = new CancelEvent(at, id);
		
		}
		else if (EventType == 'P')
		{
			Infile  >> at >> id;
			arrEvents[i] = new PromotEvent(at, id);
		
		}
	}

	Infile.close();
}

MasterNode::~MasterNode()
{
	InExecution.~LinkedList();
	CompletedProcesses.~LinkedList();
	SysWaitingList.~PriorityQueue();
	InterWaitingList.~LinkedQueue();
	CompIntenWaitingList.~LinkedQueue();
};
