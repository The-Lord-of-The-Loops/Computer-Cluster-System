#pragma once
#include "Event.h"
class ArrivalEvent :
	public Event
{
public:
	ArrivalEvent(int At, int ID, ProcessType TYP, int DL, int ET, int P, MasterNode* MN);
	void Execute() override;
	void AddtoWaitList(Process process);

	ProcessType TYP;
	int DL, ET, P;
};

