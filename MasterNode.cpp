#include "MasterNode.h"

MasterNode::MasterNode(string inputfile, string outputfile)

{
	ReadNecessaryData(inputfile);
	clock = 0;
}

bool MasterNode::Assign(Process Process)
{
	Node<Machine> *p = MachineList->Head;
	if (Process.GetProcessType() == System)
	{
		while (p)
		{
			if (p->getItem().getMachineType() == generalpurpose && p->getItem().isAvailable())
			{
				p->getItem().setAvailability(false);
				Process.SetStatus(Inprocess);
				return true;
			}
			else if (p->getItem().getMachineType() == interactive && p->getItem().isAvailable())
			{
				p->getItem().setAvailability(false);
				Process.SetStatus(Inprocess);
				return true;
			}
			else if (p->getItem().getMachineType() == GPU && p->getItem().isAvailable())
			{
				p->getItem().setAvailability(false);
				Process.SetStatus(Inprocess);
				return true;
			}
			else if (!p->getNext())
			{
				Process.SetStatus(Waiting);
				return false;
			}
			p = p->getNext();
		}
	}
	else if (Process.GetProcessType() == interactive)
	{
		while (p)
		{
			if (p->getItem().getMachineType() == interactive && p->getItem().isAvailable())
			{
				p->getItem().setAvailability(false);
				Process.SetStatus(Inprocess);
				return true;
			}
			else if (p->getItem().getMachineType() == generalpurpose && p->getItem().isAvailable())
			{
				p->getItem().setAvailability(false);
				Process.SetStatus(Inprocess);
				return true;
			}
			else if (!p->getNext())
			{
				Process.SetStatus(Waiting);
				return false;
			}
			p = p->getNext();
		}
	}
	else if (Process.GetProcessType() == ComputationallyIntensive)
	{
		while (p)
		{
			if (p->getItem().getMachineType() == GPU && p->getItem().isAvailable())
			{
				p->getItem().setAvailability(false);
				Process.SetStatus(Inprocess);
				return true;
			}
			else if (!p->getNext())
			{
				Process.SetStatus(Waiting);
				return false;
			}
			p = p->getNext();
		}
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

void MasterNode::AddtoWaitlist(Process Process)
{
	if (Process.GetProcessType() == System)
		SysWaitingList.enqueue(Process, Process.GetPriority());
	else
		InterWaitingList.enqueue(Process);
}

void MasterNode::isEmpty(bool &Running)
{
	Running = false;
	if (ProcessList->getCount() == 0)
	{
		if (SysWaitingList.isEmpty() && InterWaitingList.isEmpty() && CompIntenWaitingList.isEmpty())
		{
			if (Events->getCount() == 0)
			{
				Running = true;
			}
		}
	}
}

void MasterNode::ExecuteProcesses(int cycle, LinkedList<Process> *ToBeExecuted)
{
	if (cycle % 5 == 0)
	{
		Node<Process> *p = ToBeExecuted->getHead();
		//First Type
		ArrivalEvent *AT = dynamic_cast<ArrivalEvent *>(p->getItem());
		if ()
		{
		}
		//Second Type

		//Third Type
	}
}

void MasterNode::ReadNecessaryData(string infile)
{
	ifstream Infile;
	Infile.open(infile);
	Infile.open(infile);
	Infile >> no_GP >> no_GU >> no_IO >> rsp_GP >> rsp_GU >> rsp_IO >> N >> BGP >> BGU >> BIO >> AutoP >> E;
	//cout << " " << no_GP << " " << no_GU << " " << no_IO << " " << rsp_GP << " " << rsp_GU << " " << rsp_IO << " " << N << " " << BGP << " " << BGU << " " << BIO << " " << AutoP << " " << E;
	char EventType, processtype;
	int at, id, dl, et, p;

	for (int i = 0; i < E, i++)
	{
		Infile >> EventType;

		if (EventType == 'A')
		{
			Infile >> processtype >> at >> id >> dl >> et >> p;
			arrEvents[i] = new ArrivalEvent(at, id, (processtype == 'S') ? System : (processtype == 'I') ? Interactive : ComputationallyIntensive, dl, et, p, this); ///////////////////////////check this
		
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
