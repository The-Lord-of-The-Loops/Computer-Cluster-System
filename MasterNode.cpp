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
	Mode = ProgramUI.getProgram_Mode();
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
				process.AssignedLastCycle = true;
				process.AssignedMachine = &mach;
				return true;
			}

		if (!AV_IO_Machines.isEmpty())
		{
			AV_IO_Machines.dequeue(mach);
			mach.inextime = clock + mach.getResponseTime() + process.GetExecutionTime();
			EX_IO_Machines.enqueue(mach, mach.inextime);
			process.WT = process.WT + mach.getResponseTime();
            process.AssignedLastCycle = true;
            process.AssignedMachine = &mach;
			return true;
		}

		if (!AV_GU_Machines.isEmpty())
		{
			AV_GU_Machines.dequeue(mach);
			mach.inextime = clock + mach.getResponseTime() + process.GetExecutionTime();
			EX_GU_Machines.enqueue(mach, mach.inextime);
			process.WT = process.WT + mach.getResponseTime();
            process.AssignedLastCycle = true;
            process.AssignedMachine = &mach;
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
            process.AssignedLastCycle = true;
            process.AssignedMachine = &mach;
			return true;
		}

		if (!AV_GP_Machines.isEmpty())
		{
			AV_GP_Machines.dequeue(mach);
			mach.inextime = clock + mach.getResponseTime() + process.GetExecutionTime();
			EX_GP_Machines.enqueue(mach, mach.inextime);
            process.AssignedLastCycle = true;
            process.AssignedMachine = &mach;
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
            process.AssignedLastCycle = true;
            process.AssignedMachine = &mach;
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
			ProgramUI.PrintGPMachine(mach.getID());
			gpque.enqueue(mach, mach.getResponseTime());
		}
	}
	for (int i = 0; i < no_GU; i++)
	{
		if (AV_GU_Machines.dequeue(mach))
		{
            ProgramUI.PrintGPUMachine(mach.getID());
			guque.enqueue(mach, mach.getResponseTime());
		}
	}
	for (int i = 0; i < no_IO; i++)
	{
		if (AV_IO_Machines.dequeue(mach))
		{
            ProgramUI.PrintIOMachine(mach.getID());
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
	UI::printString("Numbers of Available Machines:     ");
	
	UI::PrintGPMachine(AV_GP_Machines.GetCount());

	UI::PrintGPUMachine(AV_GU_Machines.GetCount());

	UI::PrintIOMachine(AV_IO_Machines.GetCount());

    UI::printString("\n");
}

void MasterNode::Printno_Wa_Process()
{
    UI::printString("Numbers of Waiting Processes:      ");

    UI::PrintSystemProcess(SysWaitingList.count);

	UI::PrintInteractiveProcess(InterWaitingList.count);

	UI::PrintCompIntenProcess(CompIntenWaitingList.GetCount());

	UI::printString("\n");
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
			UI::PrintSystemProcess(process.GetID());
			p = p->getNext();
		}
	}
	p = InterWaitingList.Head;
	if (p)
	{
		for (int i = 0; i < InterWaitingList.count; i++)
		{
			process = p->getItem();
            UI::PrintInteractiveProcess(process.GetID());
			p = p->getNext();
		}
	}
		for (int i = 0; i < CompIntenWaitingList.GetCount(); i++)
		{
			CompIntenWaitingList.dequeue(process);
            UI::PrintCompIntenProcess(process.GetID());
			CompIntenWaitingList.enqueue(process);
		}
	cout << endl;
}

void MasterNode::Printno_In_Execution()
{
	int sysp =0 , interp = 0, compp = 0;
    cout << "Numbers of In-Execution Processes: ";
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
		UI::PrintSystemProcess(process.GetID());
		p = p->getNext();
	}
	p = InterInExecution.Head;
	while (p)
	{
		process = p->getItem();
        UI::PrintInteractiveProcess(process.GetID());
		p = p->getNext();
	}
	p = CompInExecution.Head;
	while (p)
	{
		process = p->getItem();
        UI::PrintCompIntenProcess(process.GetID());
		p = p->getNext();
	}
	cout << endl;
}

void MasterNode::Printno_Completed(int NoInteractive, int NoCompInt, int NoSys) {
    UI::printString("Numbers of Completed Processes:    ");

    UI::PrintInteractiveProcess(NoInteractive);

    UI::PrintCompIntenProcess(NoCompInt);

    UI::PrintSystemProcess(NoSys);

    UI::printString("\n");
}

void MasterNode::PrintCompletedIDs(int &NoInteractive, int &NoCompInt, int &NoSys) {
    NoSys = 0;
    NoInteractive = 0;
    NoCompInt = 0;
    Node<Process> *p =  CompletedProcesses.Head;
    cout << "Completed Processes:    ";
    while(p){
        switch(p->getItem().GetProcessType()){
            case Interactive:{
                NoInteractive++;
                UI::PrintInteractiveProcess(p->getItem().GetID());
                break;
            }
            case System:{
                NoSys++;
                UI::PrintSystemProcess(p->getItem().GetID());
                break;
            }
            case ComputationallyIntensive:{
                NoCompInt++;
                UI::PrintCompIntenProcess(p->getItem().GetID());
                break;
            }
        }
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
		if (queEvents.isEmpty() && SysWaitingList.isEmpty() && InterWaitingList.isEmpty() && CompIntenWaitingList.isEmpty() && SysInExecution.isEmpty() && CompInExecution.isEmpty() && InterInExecution.isEmpty())
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
    //Available Machines
    PrintAvMacIDs();
    //Waiting Processes
    PrintWaProcIDs();
    //In-Execution Processes
    PrintInExecIDs();
    //Completed Processes
    int NoInteractive, NoCompInt, NoSys;
    PrintCompletedIDs(NoInteractive, NoCompInt, NoSys);
    //Current Cycle
    ProgramUI.PrintCycle(clock);
    //No of waiting processes of each type
    Printno_Wa_Process();
    //No of Available Processes of each type
    Printno_Av_Machines();
    //Type and ID of All machines and processes assigned last cycle only
    FindAssignedLastCycle();
    //Total no. of processes completed so far of each type
    PrintCompletedIDs(NoInteractive, NoCompInt, NoSys);
}

void MasterNode::ExecuteEvents() {
	cout << "Events yet to be executed: ";
	bool dequeued = true;
	Event* ev;
	while (!queEvents.isEmpty() && dequeued)
	{
		queEvents.peek(ev);
		if (ev->ArrivalTime == clock)
		{
			ev->Execute(SysWaitingList, InterWaitingList, CompIntenWaitingList);
			dequeued = queEvents.dequeue(ev);
		}
		else
			dequeued = false;
	}
	cout << endl;
}

void MasterNode::Simulate() {
    switch(Mode){
        case PROG_MODE(0):{
            //Interactive
            bool exev = false;
            while(Check(exev)){
                Analyze(exev);
                PrintInfo();
                ProgramUI.WaitForUser();
            }
            break;
        }
        case PROG_MODE(1):{
            //Step by step
            bool exev = false;
            while(Check(exev)){
                Analyze(exev);
                PrintInfo();
            }
            break;
        }
        case PROG_MODE(2):{
            //Silent
            bool exev = false;
            while(Check(exev))
                Analyze(exev);

            break;
        }
    }
    SaveToFile();
}

bool MasterNode::Check(bool exev) {
    return !(!exev && SysWaitingList.isEmpty() && InterWaitingList.isEmpty() && CompIntenWaitingList.isEmpty() &&
             SysInExecution.isEmpty() && InterInExecution.isEmpty() && CompInExecution.isEmpty());
}

void MasterNode::FindAssignedLastCycle() {
    Node<Process>* p = SysInExecution.Head;
    while(p){
        if(p->getItem().AssignedLastCycle){
            Process process = p->getItem();
            process.NotAssignedLastCycle();
            UI::PrintAssignedProcessToMachine(process.GetProcessType(),process.GetID(),process.AssignedMachine->getMachineType(),process.AssignedMachine->getID());
        }
        p = p->getNext();
    }
    p = InterInExecution.Head;
    while(p){
        if(p->getItem().AssignedLastCycle){
            Process process = p->getItem();
            process.NotAssignedLastCycle();
            UI::PrintAssignedProcessToMachine(process.GetProcessType(),process.GetID(),process.AssignedMachine->getMachineType(),process.AssignedMachine->getID());
        }
        p = p->getNext();
    }
    p = CompInExecution.Head;
    while(p){
        if(p->getItem().AssignedLastCycle){
            Process process = p->getItem();
            process.NotAssignedLastCycle();
            UI::PrintAssignedProcessToMachine(process.GetProcessType(),process.GetID(),process.AssignedMachine->getMachineType(),process.AssignedMachine->getID());
        }
        p = p->getNext();
    }
}

