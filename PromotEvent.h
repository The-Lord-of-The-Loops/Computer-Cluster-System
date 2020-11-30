#pragma once
#include "Event.h"
class PromotEvent : public Event{
public:
	PromotEvent(int AT, int ID);
	void Execute(LinkedList<Process>& Sys, LinkedQueue<Process>& Inter, LinkedQueue<Process>& Comp) override;
	void PromoteSystemToComputationallyIntensive(int ID, LinkedList<Process>& Sys, LinkedQueue<Process>& Inter);
};

