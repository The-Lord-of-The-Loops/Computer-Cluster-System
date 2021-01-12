#pragma once
#include "Event.h"
class PromotEvent : public Event{
public:
	PromotEvent(int AT, int ID);
	void Execute(LinkedList<Process>& Sys, LinkedList<Process>& Inter, LinkedQueue<Process>& Comp) override;
	void PromoteInteractiveToSystem(int ID, LinkedList<Process>& Sys, LinkedList<Process>& Inter);
};

