#pragma once
#include "Event.h"
class PromotEvent : public Event{
public:
	PromotEvent(int AT, int ID, MasterNode* MN) : Event(AT, ID, MN) {};
	void Execute() override;
	void PromotIntertoSys(int ID);
};

