#include "MasterNode.h"

/*MasterNode::MasterNode(LinkedList<Machine> * MachineList, LinkedList<Process> * ProcessList)
{
	this->MachineList = MachineList;
	this->ProcessList = ProcessList;
}

bool MasterNode::Assign(Process Process)
{
	Node<Machine>* p = MachineList->Head;
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
			else if (!p->getNext)
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
			else if (!p->getNext)
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
			else if (!p->getNext)
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

bool MasterNode::Wait(Process Process)
{
	
	return WaitingList.enqueue(Process);
}

bool MasterNode::Promote(Process Process)
{
	return false;
}

void MasterNode::AddtoWaitlist(Process Process)
{
	if (Process.GetProcessType() == System)
		WSysQ.enqueue(Process, Process.GetPriority());
	else
		WInterIntenQ.enqueue(Process);
}

MasterNode::~MasterNode()
{
}*/
