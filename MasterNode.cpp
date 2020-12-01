#include "MasterNode.h"

MasterNode::MasterNode(string inputfile, string outputfile)
{
    SysWaitingList.count = 0;
	ReadNecessaryData(inputfile);
	clock = 0;
}

MasterNode::MasterNode()
{
    SysWaitingList.count = 0;
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
				SysInExecution.InsertLast(process);
				return true;
			}
		}

		for (int i = 0; i < no_IO; i++)
		{
			if (IO_Machines[i].isAvailable())
			{
				IO_Machines[i].setAvailability(false);
				SysInExecution.InsertLast(process);
				return true;
			}
		}

		for (int i = 0; i < no_GU; i++)
		{
			if (GU_Machines[i].isAvailable())
			{
				GU_Machines[i].setAvailability(false);
				SysInExecution.InsertLast(process);
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
				InterInExecution.InsertLast(process);
				return true;
			}
		}

		for (int i = 0; i < no_GP; i++)
		{
			if (GP_Machines[i].isAvailable())
			{
				GP_Machines[i].setAvailability(false);
				InterInExecution.InsertLast(process);
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
				CompInExecution.InsertLast(process);
				return true;
			}
		}
		return false;
	}
	return false;
}

void MasterNode::SimpleSimulation(string inputfile)
{
	ReadNecessaryData(inputfile);
	int i = 0;
	bool operate = true;
	bool nextcycle = true;
	bool exev;
	

	while (operate && nextcycle)
	{
		clock++;
		i++;
		exev = false;

		//Assigning Events
        ExecuteEvents(exev);

        //Execute 1 of each type
       ExecuteOneProcessOfEachType();
		
		
		if (clock % 5 == 0)
		{
			//CompleteEach5Cycles();
			Process process;

			if(SysInExecution.Head)
			process = SysInExecution.Head->getItem();
			CompletedProcesses.InsertLast(process);
			SysInExecution.DeleteFirst();

			if (InterInExecution.Head)
			process = InterInExecution.Head->getItem();
			CompletedProcesses.InsertLast(process);
			InterInExecution.DeleteFirst();
			if (CompInExecution.Head)
			process = CompInExecution.Head->getItem();
			CompletedProcesses.InsertLast(process);
			CompInExecution.DeleteFirst();
		}
		cout << "Cycle: " << i << endl;
		//Printing to console
		PrintInfo();

        //Checking for operations
        if (!exev && SysWaitingList.isEmpty() && InterWaitingList.isEmpty() && CompIntenWaitingList.isEmpty() && SysInExecution.isEmpty() && InterInExecution.isEmpty() && CompInExecution.isEmpty())
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
	cout << "[ " << SysWaitingList.count << " ]" << "   ";
	cout << "( " << InterWaitingList.GetCount() << " )" << "   ";
	cout << CompIntenWaitingList.GetCount() << endl;
}

void MasterNode::PrintWaProcIDs()
{
	Process process;
	Node<Process> *p;
	p = SysWaitingList.Head;
	cout << "Waiting Processes: ";
	if (p) {
		for (int i = 0; i < SysWaitingList.count; i++)
		{
			process = p->getItem();
			cout << "[ " << process.GetID() << " ]" << "  ";
			p = p->getNext();
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
	}
	cout << endl;
}

void MasterNode::Printno_In_Execution()
{
	int sysp =0 , interp = 0, compp = 0;
	cout << "Printing No. of In Execution Processes: " << endl;
	Node<Process>* p = SysInExecution.Head;
	while (p)
	{
		p = p->getNext();
		sysp++;
	}
	p = InterInExecution.Head;
	while (p)
	{
		p = p->getNext();
		interp++;
	}			
	p = CompInExecution.Head;
	while (p)
	{
		p = p->getNext();
		compp++;
	}
	cout << "[ " << sysp << " ]" << "  " << "( " << interp << " )" << "  " << compp << endl;
}

void MasterNode::PrintInExecIDs()
{
	cout << "In Execution Processes: ";
	Node<Process>* p = SysInExecution.Head;
	Process process;
	while (p)
	{
		process = p->getItem();
		cout << "[ " << process.GetID() << " ]" << "  ";
		p = p->getNext();
	}
	p = InterInExecution.Head;
	while (p)
	{
		process = p->getItem();
		cout << "( " << process.GetID() << " )" << "  ";
		p = p->getNext();
	}
	p = CompInExecution.Head;
	while (p)
	{
		process = p->getItem();
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
			if(!SysWaitingList.isEmpty()){
			    process = SysWaitingList.Head->getItem();
			    SysWaitingList.DeleteFirst();
                Assigned = Assign(process);
            }

			if (!Assigned)
				SysWaitingList.InsertSorted(process,process.GetPriority());
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
		if (!arrEvents[E] && SysWaitingList.isEmpty() && InterWaitingList.isEmpty() && CompIntenWaitingList.isEmpty() && SysInExecution.isEmpty() && CompInExecution.isEmpty() && InterInExecution.isEmpty())
		{
			operate = false;
		}
	}
}

bool MasterNode::IsExecuting()
{
	return (!SysInExecution.isEmpty() && !InterInExecution.isEmpty() && !CompInExecution.isEmpty());
}

void MasterNode::ReadNecessaryData(string infile)
{
	ifstream Infile;
	Infile.open(infile);
	Infile >> no_GP >> no_GU >> no_IO >> rsp_GP >> rsp_GU >> rsp_IO >> N >> BGP >> BGU >> BIO >> AutoP >> E;
	arrEvents = new Event*[E];
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
	
	for (int i = 0; i < E; i++)
	{
		delete arrEvents[i];
	}
	delete[] arrEvents;
}

void MasterNode::PrintInfo() {
    Printno_Av_Machines();
    PrintAvMacIDs();

    Printno_Wa_Process();
    PrintWaProcIDs();

    Printno_In_Execution();
    PrintInExecIDs();
}

void MasterNode::ExecuteEvents(bool &exev) {
	cout << "Events yet to be executed: ";
	for (int i = 0; i < E; i++)
    {
        if (arrEvents[i])
        {
           cout << arrEvents[i]->ID << " , ";
            if (arrEvents[i]->ArrivalTime == clock)
            {
				PromotEvent* test = dynamic_cast<PromotEvent*> (arrEvents[i]);
				if (!test)
					arrEvents[i]->Execute(SysWaitingList, InterWaitingList, CompIntenWaitingList);
				delete arrEvents[i];
				arrEvents[i] = nullptr;
            }
            exev = true;
        }
    }
	cout << endl;
	cout << endl;
}

void MasterNode::ExecuteOneProcessOfEachType() {
    bool dequeued;
    Process process;

    if(!SysWaitingList.isEmpty()){
        process = SysWaitingList.Head->getItem();
        SysInExecution.InsertLast(process);
		SysWaitingList.DeleteFirst();
    }

    dequeued = InterWaitingList.peek(process);
    if (dequeued)
    {
        InterWaitingList.dequeue(process);
        InterInExecution.InsertLast(process);
    }

    dequeued = CompIntenWaitingList.peek(process);
    if (dequeued)
    {
        CompIntenWaitingList.dequeue(process);
        CompInExecution.InsertLast(process);
    }
}

/*void MasterNode::CompleteEach5Cycles()
{

	Node<Process>* p;
	Node<Process>* R;
	bool Syscomp, Intercomp, compcomp;
	Process process;
	p = InExecution.Head;
	R = p;
	Syscomp = false; Intercomp = false; compcomp = false;

	if (!InExecution.isEmpty())
	{
		bool completed;
		while (!Syscomp || !Intercomp || !compcomp)
		{
			if (!p)
				return;
			completed = false;
			process = p->getItem();
			if (process.GetProcessType() == System && !Syscomp)
			{
				CompletedProcesses.InsertLast(process);
				p = p->getNext();
				InExecution.DeleteFirst();
				Syscomp = true;
				completed = true;
			}
			else if (process.GetProcessType() == Interactive && !Intercomp)
			{
				CompletedProcesses.InsertLast(process);
				p = p->getNext();
				InExecution.DeleteFirst();
				Intercomp = true;
				completed = true;
			}
			else if (process.GetProcessType() == ComputationallyIntensive && !compcomp)
			{
				CompletedProcesses.InsertLast(process);
				p = p->getNext();
				InExecution.DeleteFirst();
				compcomp = true;
				completed = true;
			}
			if (!completed)
				break;
			else if (Syscomp && Intercomp && compcomp)
				return;
		}

		R = p->getNext();
		while (!Syscomp || !Intercomp || !compcomp)
		{
			if (!R)
				return;
			completed = false;
			process = R->getItem();
			if (process.GetProcessType() == System && !Syscomp)
			{
				CompletedProcesses.InsertLast(process);
				p->setNext(R->getNext());
				delete R;
				R = p->getNext();
				Syscomp = true;
				completed = true;
			}
			else if (process.GetProcessType() == Interactive && !Intercomp)
			{
				CompletedProcesses.InsertLast(process);
				p->setNext(R->getNext());
				delete R;
				R = p->getNext();
				Intercomp = true;
				completed = true;
			}
			else if (process.GetProcessType() == ComputationallyIntensive && !compcomp)
			{
				CompletedProcesses.InsertLast(process);
				p->setNext(R->getNext());
				delete R;
				R = p->getNext();
				compcomp = true;
				completed = true;
			}
			if (!completed)
			{
				p = p->getNext();
				R = R->getNext();
			}
			else if (Syscomp && Intercomp && compcomp)
				return;
		}
	}
}
;*/