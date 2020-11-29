#pragma once
#include "Process.h"
#include "PriorityQueue.h"
#include "LinkedQueue.h"
//#include "MasterNode.h"
class Event{
public:
    int ArrivalTime;
    int ID;

	Event(int AT, int ID);
	virtual void Execute(PriorityQueue<Process>& Sys, LinkedQueue<Process>& Inter, LinkedQueue<Process>& Comp) = 0;
	~Event();
	//Getters
	int GetArrivalTime();
	//Setters
};
