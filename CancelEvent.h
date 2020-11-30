#pragma once
#include "Event.h"
class CancelEvent : public Event{
public:
	CancelEvent(int AT, int ID);
	void deletSysProcess(int ID, LinkedList<Process> &Sys);
	void Execute(LinkedList<Process>& Sys, LinkedQueue<Process>& Inter, LinkedQueue<Process>& Comp) override;
};

