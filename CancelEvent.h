#pragma once
#include "Event.h"
class CancelEvent : public Event{
public:
	CancelEvent(int AT, int ID);
	void deletSysProcess(int ID, PriorityQueue<Process> Sys);
	void Execute(PriorityQueue<Process>& Sys, LinkedQueue<Process>& Inter, LinkedQueue<Process>& Comp) override;
};

