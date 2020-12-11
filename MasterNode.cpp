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

void MasterNode::complete()
{
	bool completed = true;
	Machine machine;
	Process process;
	while (completed)
	{
		if (!SysInExecution.isEmpty()) {
			process = SysInExecution.Head->getItem();
			completed = complete(process);
		}
		else
			completed = false;
	}

	completed = true;
	while (completed)
	{
		if (!InterInExecution.isEmpty()) {
			process = InterInExecution.Head->getItem();
			completed = complete(process);
		}
		else
			completed = false;
	}

	completed = true;
	while (completed)
	{
		if (!CompInExecution.isEmpty()) {
			process = CompInExecution.Head->getItem();
			completed = complete(process);
		}
		else
			completed = false;
	}
	
	completed = true;
	while (completed)
	{
		if (!EX_GP_Machines.isEmpty()) {
			EX_GP_Machines.peek(machine);
			completed = complete(machine);
		}
		else
			completed = false;
	}

	completed = true;
	while (completed)
	{
		if (!EX_GU_Machines.isEmpty()) {
			EX_GU_Machines.peek(machine);
			completed = complete(machine);
		}
		else
			completed = false;
	}

	completed = true;
	while (completed)
	{
		if (!EX_IO_Machines.isEmpty()) {
			EX_IO_Machines.peek(machine);
			completed = complete(machine);
		}
		else
			completed = false;
	}
}

bool MasterNode::complete(Process process)
{
	bool completed = false;
	Process process;
	if (process.GetArrivalTime() + process.WT + process.GetExecutionTime() == clock)
	{
		switch (process.GetProcessType())
		{
		case System:
			process = SysInExecution.Head->getItem();
			CompletedProcesses.InsertLast(process);
			SysInExecution.DeleteFirst();
			break;

		case Interactive:
			process = InterInExecution.Head->getItem();
			CompletedProcesses.InsertLast(process);
			InterInExecution.DeleteFirst();
			break;

		case ComputationallyIntensive:
			process = CompInExecution.Head->getItem();
			CompletedProcesses.InsertLast(process);
			CompInExecution.DeleteFirst();
			break;

		}

		process.SetStatus(Complete);
		completed = true;
	}
	return completed;
}

bool MasterNode::complete(Machine machine)
{
	Machine machine;
	bool completed = false;
	if (machine.inextime == clock)
	{
		switch (machine.getMachineType()) {
		case GP:
			EX_GP_Machines.dequeue(machine);
			AV_GP_Machines.enqueue(machine, machine.getResponseTime());
			break;

		case GU:
			EX_GU_Machines.dequeue(machine);
			AV_GU_Machines.enqueue(machine, machine.getResponseTime());
			break;

		case IO:
			EX_IO_Machines.dequeue(machine);
			AV_IO_Machines.enqueue(machine, machine.getResponseTime());
			break;
		}
		completed = true;
	}
	return completed;
}

void MasterNode::dispatch()
{
	bool dispatched = true;
	Process process;
	Node<Process>* p;

	p = SysWaitingList.Head;
	while (dispatched && p)
	{
			process = p->getItem();
			dispatched = dispatch(process);
			p = p->getNext();
	}
	
	dispatched = true;
	p = InterWaitingList.Head;
	while (dispatched && p)
	{
			process = p->getItem();
			dispatched = dispatch(process);
			p = p->getNext();
	}

	dispatched = true;
	while (dispatched && !CompIntenWaitingList.isEmpty())
	{
		CompIntenWaitingList.peek(process);
		dispatched = dispatch(process);
		if (dispatched)
			CompIntenWaitingList.dequeue(process);
	}
}


bool MasterNode::dispatch(Process process)
{
	bool Assigned = false;
	if (process.getStatus() == Waiting && process.GetArrivalTime() + process.WT == clock)
	{
		Assigned = Assign(process);
		if (!Assigned)
			process.WT++;
		else
			process.SetStatus(Dispatched);
	}
	return Assigned;
}

void MasterNode::execute()
{
	bool executed = true;
	Process process;
	while (executed)
	{
		if (!SysWaitingList.isEmpty()) {
			process = SysWaitingList.Head->getItem();
			executed = execute(process);
		}
		else
			executed = false;
	}

	executed = true;
	while (executed)
	{
		if (!InterWaitingList.isEmpty()) {
			process = InterWaitingList.Head->getItem();
			executed = execute(process);
		}
		else
			executed = false;
	}

	executed = true;
	while (executed)
	{
		if (!CompIntenWaitingList.isEmpty()) {
			CompIntenWaitingList.peek(process);
			executed = execute(process);
		}
		else
			executed = false;
	}
}

bool MasterNode::execute(Process process)
{
	bool executed = false;
	if (process.getStatus() == Dispatched && process.WT + process.GetArrivalTime() == clock) {
		switch (process.GetProcessType()) {
		case System:

			SysInExecution.InsertSorted(SysWaitingList.Head->getItem(), clock + process.GetExecutionTime());
			SysWaitingList.DeleteFirst();
			break;

		case Interactive:
			InterInExecution.InsertSorted(InterWaitingList.Head->getItem(), clock + process.GetExecutionTime());
			InterWaitingList.DeleteFirst();
			break;

		case ComputationallyIntensive:

			Process temp;
			CompIntenWaitingList.dequeue(temp);
			CompInExecution.InsertSorted(temp, clock + process.GetExecutionTime());
			break;
		}
		process.SetStatus(InExecution);
		executed = true;
	}

	return executed;
}

bool MasterNode::Assign(Process process)
{
	Machine mach;
	if (process.GetProcessType() == System)
	{
		if (!AV_GP_Machines.isEmpty())
			{
				AV_GP_Machines.dequeue(mach);
				mach.inextime = clock + mach.getResponseTime() + process.GetExecutionTime();
				EX_GP_Machines.enqueue(mach, mach.inextime);
				process.WT = process.WT + mach.getResponseTime();
				return true;
			}

		if (!AV_IO_Machines.isEmpty())
		{
			AV_IO_Machines.dequeue(mach);
			mach.inextime = clock + mach.getResponseTime() + process.GetExecutionTime();
			EX_IO_Machines.enqueue(mach, mach.inextime);
			process.WT = process.WT + mach.getResponseTime();
			return true;
		}

		if (!AV_GU_Machines.isEmpty())
		{
			AV_GU_Machines.dequeue(mach);
			mach.inextime = clock + mach.getResponseTime() + process.GetExecutionTime();
			EX_GU_Machines.enqueue(mach, mach.inextime);
			process.WT = process.WT + mach.getResponseTime();
			return true;
		}

		return false;
	}
	else if (process.GetProcessType() == Interactive)
	{

		if (!AV_IO_Machines.isEmpty())
		{
			AV_IO_Machines.dequeue(mach);
			mach.inextime = clock + mach.getResponseTime() + process.GetExecutionTime();
			EX_IO_Machines.enqueue(mach, mach.inextime);
			return true;
		}

		if (!AV_GP_Machines.isEmpty())
		{
			AV_GP_Machines.dequeue(mach);
			mach.inextime = clock + mach.getResponseTime() + process.GetExecutionTime();
			EX_GP_Machines.enqueue(mach, mach.inextime);
			return true;
		}
		return false;
	}
	else if (process.GetProcessType() == ComputationallyIntensive)
	{
		if (!AV_GU_Machines.isEmpty())
		{
			AV_GU_Machines.dequeue(mach);
			mach.inextime = clock + mach.getResponseTime() + process.GetExecutionTime();
			EX_GU_Machines.enqueue(mach, mach.inextime);
			return true;
		}

		return false;
	}

	return false;
}

/*void MasterNode::SimpleSimulation(string inputfile)
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
}*/

void MasterNode::PrintAvMacIDs()
{
	Machine mach;
	PriorityQueue<Machine> gpque, guque, ioque;
	cout << "Available Machines: ";
	for (int i = 0; i < AV_GP_Machines.GetCount(); i++)
	{
		if (AV_GP_Machines.dequeue(mach))
		{
			cout << "[ " << mach.getID() << " ]" << "   ";
			gpque.enqueue(mach, mach.getResponseTime());
		}
	}
	for (int i = 0; i < no_GU; i++)
	{
		if (AV_GU_Machines.dequeue(mach))
		{
			cout << "( " << mach.getID() << " )" << "   ";
			guque.enqueue(mach, mach.getResponseTime());
		}
	}
	for (int i = 0; i < no_IO; i++)
	{
		if (AV_IO_Machines.dequeue(mach))
		{
			cout << mach.getID() << "   ";
			ioque.enqueue(mach, mach.getResponseTime());
		}
	}
	cout << endl;

	while (!gpque.isEmpty())
	{
		gpque.dequeue(mach);
		AV_GP_Machines.enqueue(mach,mach.getResponseTime());
	}
	while (!guque.isEmpty())
	{
		guque.dequeue(mach);
		AV_GU_Machines.enqueue(mach, mach.getResponseTime());
	}
	while (!ioque.isEmpty())
	{
		ioque.dequeue(mach);
		AV_IO_Machines.enqueue(mach, mach.getResponseTime());
	}
}

void MasterNode::Printno_Av_Machines()
{
	int sum = AV_GP_Machines.GetCount() + AV_GU_Machines.GetCount() + AV_IO_Machines.GetCount();
	cout << "Machines: " << sum << "  ";
	
	cout << "[ " << AV_GP_Machines.GetCount() << " ]" << "   ";
	
	cout << "( " << AV_GU_Machines.GetCount() << " )" << "   ";
	
	cout << AV_IO_Machines.GetCount() << endl;
}

void MasterNode::Printno_Wa_Process()
{
	cout << "Processes: " << SysWaitingList.count + InterWaitingList.count + CompIntenWaitingList.GetCount() << "  ";
	cout << "[ " << SysWaitingList.count << " ]" << "   ";
	cout << "( " << InterWaitingList.count << " )" << "   ";
	cout << CompIntenWaitingList.GetCount() << endl;
}

void MasterNode::PrintWaProcIDs()
{
	Process process;
	Node<Process> *p;
	cout << "Waiting Processes: ";
	p = SysWaitingList.Head;
	if (p)
	{
		for (int i = 0; i < SysWaitingList.count; i++)
		{
			process = p->getItem();
			cout << "[ " << process.GetID() << " ]" << "  ";
			p = p->getNext();
		}
	}
	p = InterWaitingList.Head;
	if (p)
	{
		for (int i = 0; i < InterWaitingList.count; i++)
		{
			process = p->getItem();
			cout << "( " << process.GetID() << " )" << "  ";
			p = p->getNext();
		}
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
		ExecuteEvents();

		//Executing Processes
	

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
    //cout << " " << no_GP << " " << no_GU << " " << no_IO << " " << rsp_GP << " " << rsp_GU << " " << rsp_IO << " " << N << " " << BGP << " " << BGU << " " << BIO << " " << AutoP << " " << E;
	//creating Machines
	for (int i = 0; i < no_GP; i++)
	{
		Machine GpMach(i, rsp_GP, N, BGP);
		AV_GP_Machines.enqueue(GpMach,GpMach.getResponseTime());
	}
	for (int i = 0; i < no_GU; i++)
	{
		Machine GuMach(i + no_GP, rsp_GU, N, BGU);
		AV_GP_Machines.enqueue(GuMach, GuMach.getResponseTime());
	}
	for (int i = 0; i < no_IO; i++)
	{
		Machine IoMach(i + no_GP + no_GU, rsp_IO, N, BIO);
		AV_GP_Machines.enqueue(IoMach, IoMach.getResponseTime());
	}
	char EventType, processtype;
	int at, id, dl, et, p;

	ev = new Event * [E];
		for (int i = 0; i < E; i++)
	{
		Infile >> EventType;

		if (EventType == 'A')
		{
			Infile >> processtype >> at >> id >> dl >> et >> p;
			ev[i] = new ArrivalEvent(at, id, (processtype == 'S') ? System : (processtype == 'I') ? Interactive : ComputationallyIntensive, dl, et, p);
			queEvents.enqueue(ev[i]);
		}
		else if (EventType == 'X')
		{
			Infile  >> at >> id;
			ev[i] = new CancelEvent(at, id);
			queEvents.enqueue(ev[i]);
		}
		else if (EventType == 'P')
		{
			Infile  >> at >> id;
			ev[i] = new PromotEvent(at, id);
			queEvents.enqueue(ev[i]);
		}
	}

	Infile.close();
}

MasterNode::~MasterNode()
{
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
	bool dequeued;
	Event* ev;
	while (!queEvents.isEmpty() && dequeued)
	{
		queEvents.peek(ev);
		if (ev->ArrivalTime == clock)
			{
				ev->Execute(SysWaitingList, InterWaitingList, CompIntenWaitingList);
				dequeued = queEvents.dequeue(ev);
			}
	}
	cout << endl;
}

/*void MasterNode::ExecuteOneProcessOfEachType() {
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
}*/

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