#pragma once
enum MachineType {
	GP,
	IO,
	GU
};

class Machine
{
private:
	int ID;
	MachineType Type;
	int RebootDuration, ResponseTime, N_bresp;
	bool Available;
public:
	//Machine();
	Machine();
	Machine(int ID, int rsp, int N, int B);

	int inextime;
	//setters
	void setRebootDuration(int RebootDuration);
	void setRebsponseTime(int ResponseTime);
	void setMachineType(MachineType Type);
	void setAvailability(bool available);
	void setNbresp(int N);
	//Getters
	int getNbresp();
	int getRebootDuration();
	int getResponseTime();
	int getID();
	bool isAvailable();
	MachineType getMachineType();

	~Machine();
};
