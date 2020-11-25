#pragma once
#include "Event.h"
class ArrivalEvent :
	public Event
{
public:
	ArrivalEvent(int At, int ID, ProcessType TYP, int DL, int ET, int P);
	void Execute() override;
	void AddtoWaitList(Process process);
private:
	ProcessType TYP;
	int DL, ET, P;
};

