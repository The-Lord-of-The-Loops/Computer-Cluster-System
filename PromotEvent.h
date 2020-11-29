#pragma once
#include "Event.h"
class PromotEvent : public Event{
public:
	PromotEvent(int AT, int ID);
	void Execute(PriorityQueue<Process>& Sys, LinkedQueue<Process>& Inter, LinkedQueue<Process>& Comp) override;
	void PromotIntertoSys(int ID, PriorityQueue<Process>& Sys, LinkedQueue<Process>& Inter);
};

