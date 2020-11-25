#include "Event.h"

Event::Event(int AT, int ID, MasterNode* MN)
{
	this->AT = AT;
	this->ID = ID;
	this->MN = MN;
}

Event::~Event()
{
}
