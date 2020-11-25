#pragma once
#include "MasterNode.h"
class Event
{
public:
	Event(int AT, int ID, MasterNode* MN);
	virtual void Execute() = 0;
	~Event();
	int AT;
	int ID;
	MasterNode* MN;
};
