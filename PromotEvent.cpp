#include "PromotEvent.h"

PromotEvent::PromotEvent(int AT, int ID) : Event(AT, ID)
{
}

void PromotEvent::Execute(LinkedList<Process> &Sys, LinkedQueue<Process> &Inter, LinkedQueue<Process> &Comp)
{
	//PromoteSystemToComputationallyIntensive(ID, Sys, Comp);
}

void PromotEvent::PromoteSystemToComputationallyIntensive(int ID, LinkedList<Process> &Sys, LinkedQueue<Process> &Inter)
{
	Process tempProcess;
	LinkedQueue<Process> Temp;
	while (!Inter.isEmpty())
	{
		Inter.dequeue(tempProcess);
		if (ID != tempProcess.GetID())
			Temp.enqueue(tempProcess);
		else
			Sys.InsertSorted(tempProcess, tempProcess.GetPriority());
	}
	while (!Temp.isEmpty())
	{
		Temp.dequeue(tempProcess);
		Inter.enqueue(tempProcess);
	}
}
