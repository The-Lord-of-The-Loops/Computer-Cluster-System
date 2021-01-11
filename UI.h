#pragma once

#include <string>
#include "PriorityQueue.h"
#include "Machine.h"
#include "Process.h"

using namespace std;

enum PROG_MODE
{
    interactive,
    StepByStep,
    Silent
};
class UI
{
public:
    //Variables

    //Fn
    UI();

    // it is actually setter !
    static PROG_MODE getProgram_Mode();
    static void printString(string text);
    //waits for any input
    static void WaitForUser();
    // puases the program in milliseconds
    static void sleep(int milliseconds);

    static void PrintNumberOfAvailableMachines(int AV_GP_Machines, int AV_GU_Machines, int AV_IO_Machines);
    static void PrintGPMachine(int ID);
    static void PrintGPUMachine(int ID);
    static void PrintIOMachine(int ID);
    static void PrintSystemProcess(int ID);
    static void PrintInteractiveProcess(int ID);
    static void PrintCompIntenProcess(int ID);
    static void PrintCycle(int Cycle);
    static void PrintAssignedProcessToMachine(ProcessType processType, int ProcessID, MachineType machineType, int MachineID);
};