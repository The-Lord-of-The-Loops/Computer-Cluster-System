#include "Machine.h"

Machine::Machine(int ID, int rsp, int N, int B)
{ 
	this->ID = ID;
	ResponseTime = rsp;
	RebootDuration = B;
	N_bresp = N;
	Available = true;
}

void Machine::setRebootDuration(int RebootDuration)
{
	this->RebootDuration = RebootDuration;
}

void Machine::setRebsponseTime(int ResponseTime)
{
	this->ResponseTime = ResponseTime;
}

void Machine::setMachineType(MachineType Type)
{
	this->Type = Type;
}

void Machine::setAvailability(bool available)
{
	Available = available;
}

void Machine::setNbresp(int N)
{
	N_bresp = N;
}

int Machine::getNbresp()
{
	return N_bresp;
}

int Machine::getRebootDuration()
{
	return RebootDuration;
}

int Machine::getResponseTime()
{
	return ResponseTime;
}

int Machine::getID()
{
	return ID;
}

bool Machine::isAvailable()
{
	return Available;
}

MachineType Machine::getMachineType()
{
	return Type;
}

Machine::~Machine()
{
}
