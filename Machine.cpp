#include "Machine.h"

Machine::Machine() { }

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

int Machine::getRebootDuration()
{
	return RebootDuration;
}

int Machine::getResponseTime()
{
	return ResponseTime;
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
