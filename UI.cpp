#include "UI.h"
#include <iostream>
#include <unistd.h>
#include "MasterNode.h"

void UI::printString(string text) {
    cout << text;
}

void UI::WaitForUser() {
    cout << "Program paused, Please enter 1 to continue: " << endl;
    int x;
    cin >> x;
}

PROG_MODE UI::getProgram_Mode() {
    cout << "Please choose mode: \n1: Interactive \n2: Step by Step \n3: Silent" << endl;
    int choice;
    cin >> choice;
    if(choice == 1){
        return interactive;
    } else if (choice == 2){
        return StepByStep;
    } else if (choice == 3){
        return Silent;
    }
    return interactive;
}

void UI::sleep(int milliseconds) {
    usleep(milliseconds*1000);
}


void UI::PrintNumberOfAvailableMachines(int AV_GP_Machines,int AV_GU_Machines,int AV_IO_Machines) {
    cout << "Numbers of Available Machines:     ";

    cout << "[ " << AV_GP_Machines << " ]" << "   ";

    cout << "(" << AV_GU_Machines << ")" << "   ";

    cout << AV_IO_Machines << endl;
}

void UI::PrintGPMachine(int ID) {
    cout << "[ " << ID << " ]" << "   ";
}

void UI::PrintGPUMachine(int ID) {
    cout << "( " << ID << " )" << "   ";
}

void UI::PrintIOMachine(int ID) {
    cout << ID << "   ";
}

void UI::PrintCycle(int Cycle) {
    cout << "Current cycle: " << Cycle << endl;
}

void UI::PrintSystemProcess(int ID) {
    cout << "[ " << ID << " ]" << "   ";
}

void UI::PrintInteractiveProcess(int ID) {
    cout << "( " << ID << " )" << "   ";
}

void UI::PrintCompIntenProcess(int ID) {
    cout << ID << "   ";
}

void UI::PrintAssignedProcessToMachine(ProcessType processType, int ProcessID, MachineType machineType, int MachineID) {
    switch(machineType){
        case GP:{
            UI::printString("GP");
            break;
        }
        case IO:{
            UI::printString("IO");
            break;
        }
        case GU:{
            UI::printString("GU");
            break;
        }
    }
    UI::printString(to_string(MachineID) + "->");

    switch(processType){
        case Interactive:{
            UI::printString("I");
            break;
        }
        case System:{
            UI::printString("S");
            break;
        }
        case ComputationallyIntensive:{
            UI::printString("CI");
            break;
        }
    }
    UI::printString(to_string(ProcessID));
}

UI::UI() = default;

