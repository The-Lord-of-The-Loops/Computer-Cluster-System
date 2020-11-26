#include "Machine.h"

Machine::Machine(MachineType Type, int RebootDuration, int ResponseTime)
{
	this->Type = Type;
	this->RebootDuration = RebootDuration;
	this->ResponseTime = ResponseTime;
}

void Machine::setRebootDuration(int RebootDuration)
{
	this->setRebootDuration = RebootDuration;
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
