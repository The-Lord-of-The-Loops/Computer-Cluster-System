#pragma once
#include "Event.h"
class CancelEvent :
	public Event
{
public:
	CancelEvent(int AT, int ID, MasterNode* MN) :Event(AT, ID, MN) {};
	void deletSysProcess(int ID);
	void Execute() override;
};

