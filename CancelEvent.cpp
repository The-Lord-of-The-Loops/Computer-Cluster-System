#include "CancelEvent.h"

CancelEvent::CancelEvent(int AT, int ID) : Event(AT,ID){}

void CancelEvent::deletSysProcess(int ID, PriorityQueue<Process> Sys)
{
	Process tempProcess;
	PriorityQueue<Process> Temp;
	while (!Sys.isEmpty())
	{
		Sys.dequeue(tempProcess);
		if (ID != tempProcess.GetID())
			Temp.enqueue(tempProcess, tempProcess.GetPriority());
	}
	while (!Temp.isEmpty())
	{
		Temp.dequeue(tempProcess);
		Sys.enqueue(tempProcess, tempProcess.GetPriority());
	}
}

void CancelEvent::Execute(PriorityQueue<Process>& Sys, LinkedQueue<Process>& Inter, LinkedQueue<Process>& Comp)
{
	deletSysProcess(ID, Sys);
}
