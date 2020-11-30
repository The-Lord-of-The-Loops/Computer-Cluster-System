#pragma once
#include "Event.h"
class ArrivalEvent : public Event
{
public:
	ArrivalEvent(int At, int ID, ProcessType TYP, int DL, int ET, int P);
	void Execute(LinkedList<Process> &Sys, LinkedQueue<Process> &Inter, LinkedQueue<Process> &Comp) override;
	void AddtoWaitList(Process process, LinkedList<Process> &Sys, LinkedQueue<Process> &Inter, LinkedQueue<Process> &Comp);

	ProcessType TYP;
	int DL, ET, P;
};
