#include "MasterNode.h"

MasterNode::MasterNode(LinkedList<Machine> * MachineList, LinkedList<Process> * ProcessList,LinkedList<Event> *Events)
{
    this->Events = Events;
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

void MasterNode::isEmpty(bool& Running) {
    Running = false;
    if(ProcessList->getCount() == 0){
        if(SysWaitingList.isEmpty() && InterWaitingList.isEmpty() && CompIntenWaitingList.isEmpty()){
            if(Events->getCount() == 0){
                Running = true;
            }
        }
    }

}

void MasterNode::ExecuteProcesses(int cycle, LinkedList<Process> *ToBeExecuted) {
    if(cycle%5 == 0){
        Node<Process>* p = ToBeExecuted->getHead();
        //First Type
        ArrivalEvent* AT = dynamic_cast<ArrivalEvent*>(p->getItem());
        if(){

        }
        //Second Type

        //Third Type

    }
}

void MasterNode::ReadEvents(int cycle) {
    Node<Event>* p = Events->getHead();
    while(p){
        ArrivalEvent* AT = dynamic_cast<ArrivalEvent>(p->getItem());
    }
}

MasterNode::~MasterNode() = default;
