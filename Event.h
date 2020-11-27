#pragma once
#include "MasterNode.h"
class Event{
public:
    int ArrivalTime;
    int ID;
    MasterNode* MN;

	Event(int AT, int ID, MasterNode* MN);
	virtual void Execute() = 0;
	~Event();
	//Getters
	int GetArrivalTime();
	//Setters
};
