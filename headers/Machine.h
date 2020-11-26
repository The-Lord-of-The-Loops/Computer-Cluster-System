#pragma once
enum MachineType {
	generalpurpose,
	interactive,
	GPU
};

class Machine
{
private:
	MachineType Type;
	int RebootDuration, ResponseTime;
	bool Available;
public:
	Machine(MachineType Type, int RebootDuration, int ResponseTime);

	//setters
	void setRebootDuration(int RebootDuration);
	void setRebsponseTime(int ResponseTime);
	void setMachineType(MachineType Type);
	void setAvailability(bool available);

	//Getters
	int getRebootDuration();
	int getResponseTime();
	bool isAvailable();
	MachineType getMachineType();

	~Machine();
};
