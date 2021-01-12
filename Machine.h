#pragma once
enum MachineType
{
	GP,
	IO,
	GU
};

class Machine
{
private:

    int ID;
    MachineType Type;
    int RebootDuration, ResponseTime;
    int N_beforeReboot; // number of processes till reboot.
    bool Available;
public:

    //Machine();
	Machine();
    Machine(int ID, int rsp, int N, int B, MachineType type);
    int MaintenanceTime = 5;
    int NoOfProcessesToMaintenance = 2;
    int LateResponseDelayInCycles = 2;
    int NoOfProcessesLeftTillMaintenance;
    int MaintenanceTimeLeft = 0;
    int NoProcesses;

	int inextime;
    //setters
	void setRebootDuration(int RebootDuration);
    void setRebsponseTime(int ResponseTime);
    void setMachineType(MachineType Type);
    void setAvailability(bool available);
    void setN_beforeReboot(int N);

	//Getters
	int getN_beforeReboot();
    int getRebootDuration();
    int getResponseTime();
    int getID();
    bool isAvailable();
    MachineType getMachineType();

	~Machine();
};
