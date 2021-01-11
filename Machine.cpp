#include "Machine.h"

Machine::Machine()
{
}

Machine::Machine(int ID, int rsp, int N, int B, MachineType type)
{ 
	this->ID = ID;
	ResponseTime = rsp;
	RebootDuration = B;
	N_beforeReboot = N;
	Available = true;
	inextime = 0;
	this->Type = type;
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

void Machine::setN_beforeReboot(int N)
{
	N_beforeReboot = N;
}

int Machine::getN_beforeReboot()
{
	return N_beforeReboot;
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