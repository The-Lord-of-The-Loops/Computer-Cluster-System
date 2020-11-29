#include "Event.h"

Event::Event(int AT, int ID)
{
	this->ArrivalTime = AT;
	this->ID = ID;
}

Event::~Event()
{
}

int Event::GetArrivalTime() {
    return ArrivalTime;
}
