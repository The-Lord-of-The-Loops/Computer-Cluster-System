#include "Event.h"

Event::Event(int AT, int ID, MasterNode* MN)
{
	this->ArrivalTime = AT;
	this->ID = ID;
	this->MN = MN;
}

Event::~Event()
{
}

int Event::GetArrivalTime() {
    return ArrivalTime;
}
