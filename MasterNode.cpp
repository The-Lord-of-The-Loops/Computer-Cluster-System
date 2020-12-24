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
	/*
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
	*/
}

bool MasterNode::complete(Process &process)
{
	bool completed = false;
	if (process.GetArrivalTime() + process.WT + process.GetExecutionTime() == clock)
	{
		switch (process.GetProcessType())
		{
		case System:
			process = SysInExecution.Head->getItem();
			CompletedProcesses.InsertLast(process);
			complete(process.AssignedMachineID,process.AssignedMachineType);
			SysInExecution.DeleteFirst();
			break;

		case Interactive:
			process = InterInExecution.Head->getItem();
			CompletedProcesses.InsertLast(process);
			complete(process.AssignedMachineID, process.AssignedMachineType);
			InterInExecution.DeleteFirst();
			break;

		case ComputationallyIntensive:
			process = CompInExecution.Head->getItem();
			CompletedProcesses.InsertLast(process);
			complete(process.AssignedMachineID, process.AssignedMachineType);
			CompInExecution.DeleteFirst();
			break;

		}

		process.SetStatus(Complete);
		completed = true;
	}
	return completed;
}

bool MasterNode::complete(int ID, MachineType type)
{
	bool completed = false;
	Machine machine;
		switch (type) {
		case GP:
			EX_GP_Machines.dequeue(machine);
			if(machine.getID() == ID)
				AV_GP_Machines.enqueue(machine, machine.getResponseTime());
			else
				EX_GP_Machines.enqueue(machine, machine.getResponseTime());
			break;

		case GU:
			EX_GU_Machines.dequeue(machine);
			if (machine.getID() == ID)
				AV_GU_Machines.enqueue(machine, machine.getResponseTime());
			else
				EX_GU_Machines.enqueue(machine, machine.getResponseTime());
			break;

		case IO:
			EX_IO_Machines.dequeue(machine);
			if (machine.getID() == ID)
				AV_IO_Machines.enqueue(machine, machine.getResponseTime());
			else
				EX_IO_Machines.enqueue(machine, machine.getResponseTime());
			break;
		}
		completed = true;
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
			p = p->getNext();
			dispatched = dispatch(process);
	}
	
	dispatched = true;
	p = InterWaitingList.Head;
	while (dispatched && p)
	{
			process = p->getItem();
			p = p->getNext();
			dispatched = dispatch(process);
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


bool MasterNode::dispatch(Process &process)
{
	bool Assigned = false;
	if (process.getStatus() == Waiting && process.GetArrivalTime() + process.WT + process.GetDispatchLatency() == clock)
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
	Process process;
	Node<Process>* p = SysInExecution.Head;
	while (p)
	{
	    process = p->getItem();
		p = p->getNext();
	    execute(process);
	}

	p = InterInExecution.Head;
	while (p)
	{
	    process = p->getItem();
		p = p->getNext();
		execute(process);
	}

    p = CompInExecution.Head;
	while (p)
	{
	    process = CompInExecution.Head->getItem();
		p = p->getNext();
	    execute(process);
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

bool MasterNode::Assign(Process &process)
{
	Machine mach;
	if (process.GetProcessType() == System)
	{
		if (!AV_GP_Machines.isEmpty())
			{
				AV_GP_Machines.dequeue(mach);
				mach.inextime = clock + process.GetDispatchLatency() + mach.getResponseTime() + process.GetExecutionTime();
				EX_GP_Machines.enqueue(mach, mach.inextime);
				process.WT = process.WT + process.GetDispatchLatency() + mach.getResponseTime();
				process.AssignedLastCycle = true;
				process.AssignedMachineID = mach.getID();
				process.AssignedMachineType = mach.getMachineType();
				SysInExecution.InsertSorted(process,process.GetPriority());
				SysWaitingList.DeleteNode(process.GetID());
				return true;
			}

		if (!AV_IO_Machines.isEmpty())
		{
			AV_IO_Machines.dequeue(mach);
			mach.inextime = clock + process.GetDispatchLatency() + mach.getResponseTime() + process.GetExecutionTime();
			EX_IO_Machines.enqueue(mach, mach.inextime);
			process.WT = process.WT + process.GetDispatchLatency() + mach.getResponseTime();
            process.AssignedLastCycle = true;
			process.AssignedMachineID = mach.getID();
			process.AssignedMachineType = mach.getMachineType();
            SysInExecution.InsertSorted(process,process.GetPriority());
            SysWaitingList.DeleteNode(process.GetID());
			return true;
		}

		if (!AV_GU_Machines.isEmpty())
		{
			AV_GU_Machines.dequeue(mach);
			mach.inextime = clock + process.GetDispatchLatency() + mach.getResponseTime() + process.GetExecutionTime();
			EX_GU_Machines.enqueue(mach, mach.inextime);
			process.WT = process.WT + process.GetDispatchLatency() + mach.getResponseTime();
            process.AssignedLastCycle = true;
			process.AssignedMachineID = mach.getID();
			process.AssignedMachineType = mach.getMachineType();
            SysInExecution.InsertSorted(process,process.GetPriority());
            SysWaitingList.DeleteNode(process.GetID());
			return true;
		}

		return false;
	}
	else if (process.GetProcessType() == Interactive)
	{

		if (!AV_IO_Machines.isEmpty())
		{
			AV_IO_Machines.dequeue(mach);
			mach.inextime = clock + process.GetDispatchLatency() + mach.getResponseTime() + process.GetExecutionTime();
			EX_IO_Machines.enqueue(mach, mach.inextime);
			process.WT = process.WT + process.GetDispatchLatency() + mach.getResponseTime();
            process.AssignedLastCycle = true;
			process.AssignedMachineID = mach.getID();
			process.AssignedMachineType = mach.getMachineType();
            InterInExecution.InsertSorted(process,process.GetPriority());
            InterWaitingList.DeleteNode(process.GetID());
			return true;
		}

		if (!AV_GP_Machines.isEmpty())
		{
			AV_GP_Machines.dequeue(mach);
			mach.inextime = clock + process.GetDispatchLatency() + mach.getResponseTime() + process.GetExecutionTime();
			EX_GP_Machines.enqueue(mach, mach.inextime);
			process.WT = process.WT + process.GetDispatchLatency() + mach.getResponseTime();
            process.AssignedLastCycle = true;
			process.AssignedMachineID = mach.getID();
			process.AssignedMachineType = mach.getMachineType();
            InterInExecution.InsertSorted(process,process.GetPriority());
            InterWaitingList.DeleteNode(process.GetID());
			return true;
		}
		return false;
	}
	else if (process.GetProcessType() == ComputationallyIntensive)
	{
		if (!AV_GU_Machines.isEmpty())
		{
			AV_GU_Machines.dequeue(mach);
			mach.inextime = clock + process.GetDispatchLatency() + mach.getResponseTime() + process.GetExecutionTime();
			EX_GU_Machines.enqueue(mach, mach.inextime);
			process.WT = process.WT + process.GetDispatchLatency() + mach.getResponseTime();
            process.AssignedLastCycle = true;
			process.AssignedMachineID = mach.getID();
			process.AssignedMachineType = mach.getMachineType();
            CompInExecution.InsertSorted(process,process.GetPriority());
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
	while (!AV_GP_Machines.isEmpty())
	{
		AV_GP_Machines.dequeue(mach);
		UI::PrintGPMachine(mach.getID());
		gpque.enqueue(mach, mach.getResponseTime());
	}

	while (!AV_GU_Machines.isEmpty())
	{
		AV_GU_Machines.dequeue(mach);
		UI::PrintGPUMachine(mach.getID());
		guque.enqueue(mach, mach.getResponseTime());
	}
	while (!AV_IO_Machines.isEmpty())
	{
		AV_IO_Machines.dequeue(mach);
		UI::PrintIOMachine(mach.getID());
		ioque.enqueue(mach, mach.getResponseTime());
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
	for (int i = 1; i < no_GP + 1; i++)
	{
		Machine GpMach(i, rsp_GP, N, BGP, GP);
		AV_GP_Machines.enqueue(GpMach,GpMach.getResponseTime());
	}
	for (int i = 1; i < no_GU + 1; i++)
	{
		Machine GuMach(i + no_GP, rsp_GU, N, BGU,GU);
		AV_GU_Machines.enqueue(GuMach, GuMach.getResponseTime());
	}
	for (int i = 1; i < no_IO + 1; i++)
	{
		Machine IoMach(i + no_GP + no_GU, rsp_IO, N, BIO,IO);
		AV_IO_Machines.enqueue(IoMach, IoMach.getResponseTime());
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
    UI::PrintCycle(clock);
    //No of waiting processes of each type
    Printno_Wa_Process();
    //No of Available Processes of each type
    Printno_Av_Machines();
    //Type and ID of All machines and processes assigned last cycle only
    FindAssignedLastCycle();
    //Total no. of processes completed so far of each type
    Printno_Completed(NoInteractive, NoCompInt, NoSys);
}

void MasterNode::ExecuteEvents(bool &exev) {
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
	if(!SilentCheck())
	    UI::printString("\n");
}

void MasterNode::Simulate(string path) {
    ReadNecessaryData(std::move(path));
	PrintInfo();
    Mode = UI::getProgram_Mode();
    switch(Mode){
        case PROG_MODE(0):{
            //Interactive
            bool exev = true;
            while(Check(exev)){
                Analyze(exev);
                PrintInfo();
                UI::WaitForUser();
            }
            break;
        }
        case PROG_MODE(1):{
            //Step by step
            bool exev = true;
            while(Check(exev)){
                Analyze(exev);
                UI::sleep(1000);
                PrintInfo();
            }
            break;
        }
        case PROG_MODE(2):{
            //Silent
            bool exev = true;
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
	cout << "Assigned Last cycle: ";
	Node<Process>* p = SysInExecution.Head;
    while(p){
        if(p->getItem().AssignedLastCycle){
            Process process = p->getItem();
            p->getItem().NotAssignedLastCycle();
            UI::PrintAssignedProcessToMachine(process.GetProcessType(),process.GetID(),process.AssignedMachineType,process.AssignedMachineID);
        }
        p = p->getNext();
    }
    p = InterInExecution.Head;
    while(p){
        if(p->getItem().AssignedLastCycle){
            Process process = p->getItem();
			p->getItem().NotAssignedLastCycle();
			UI::PrintAssignedProcessToMachine(process.GetProcessType(), process.GetID(), process.AssignedMachineType, process.AssignedMachineID);
		}
        p = p->getNext();
    }
    p = CompInExecution.Head;
    while(p){
        if(p->getItem().AssignedLastCycle){
            Process process = p->getItem();
			p->getItem().NotAssignedLastCycle();
			UI::PrintAssignedProcessToMachine(process.GetProcessType(), process.GetID(), process.AssignedMachineType, process.AssignedMachineID);
		}
        p = p->getNext();
    }
	cout << endl;
}

void MasterNode::Analyze(bool &exev) {
    //Reading data
    clock++;
    ExecuteEvents(exev);
    //dispatch processes
    dispatch();
    //Complete processes
    complete();
	//

}

void MasterNode::SaveToFile() {
    //Empty for now
}

bool MasterNode::SilentCheck() {
    return (Mode == Silent);
}
