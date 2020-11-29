#include "PromotEvent.h"

PromotEvent::PromotEvent(int AT, int ID) : Event(AT, ID)
{
}

void PromotEvent::Execute(PriorityQueue<Process>& Sys, LinkedQueue<Process>& Inter, LinkedQueue<Process>& Comp)
{
	PromotIntertoSys(ID, Sys, Comp);
}

void PromotEvent::PromotIntertoSys(int ID, PriorityQueue<Process>& Sys, LinkedQueue<Process>& Inter)
{
	Process tempProcess;
	LinkedQueue<Process> Temp;
	while (!Inter.isEmpty())
	{
		Inter.dequeue(tempProcess);
		if (ID != tempProcess.GetID())
			Temp.enqueue(tempProcess);
		else
			Sys.enqueue(tempProcess, tempProcess.GetPriority());
	}
	while (!Temp.isEmpty())
	{
		Temp.dequeue(tempProcess);
		Inter.enqueue(tempProcess);
	}
}
