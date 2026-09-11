#include "cpu_required.hpp"

CPU::CPU() {
    executionState = INITIALIZED;
    canExecute = true;
    RAMsize = 0;
    ROMsize = 0;
    programSize = 0;
}

CPU::~CPU() {
    ram.clear();
    rom.clear();
}

CPUes CPU::GetExecutionState() {
    return executionState;
}


CPUes CPU::Execute() {
    if (canExecute) ExecuteInstruction();
    return executionState;
}

CPUes CPU::ExecuteIgnoreCPUExecState() {
    ExecuteInstruction();
    return executionState;
}

void CPU::SetExecutionState(CPUes errorCode, bool isFatal) {
    executionState = errorCode;
    canExecute = !isFatal;
}

// Returns 1 if the file encountered an error while loading to RAM.
bool CPU::LoadFileToROM(std::string filePath) {
    std::ifstream binFile(filePath.c_str());
    if(!binFile.is_open()) return 1;
    size i = 0;
    char dataByte = 0;
    programSize = 0;
    while(!binFile.eof()) {
        if (i >= ROMsize) {
            SetExecutionState(CPUes::OUT_OF_STORAGE, false);
            break;
        }
        binFile.get(dataByte);
        rom[i++] = dataByte;
        programSize++;
    }
    binFile.close();
    executionState = CPUes::READY;
    return 0;
}