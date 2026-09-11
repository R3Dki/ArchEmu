#pragma once
#include "../../lib/types.hpp"

#include <string>
#include <fstream>
#include <iostream>

#define CPU_REQUIRED_LIB_VERSION_MAJOR 1
#define CPU_REQUIRED_LIB_VERSION_MINOR 0
#define CPU_REQUIRED_LIB_VERSION_PATCH 0

#define UNLIMITED_IPS false
#define CPU_DEBUG_MODE true

template<typename T> inline void zeroArray(T arr, size arraySize) {
    for (size i = 0; i < arraySize; i++) arr[i] = 0;
}

// CPU class, contains necessary requirements that are needed to run every CPU. DO NOT USE DIRECTLY, IT ONLY SERVES AS A SUPERCLASS.
class CPU {
    private:
    
    public:
        CPU();
        virtual ~CPU();

        CPUes executionState;
        bool canExecute;

        IPS instructionsPerSecond = 0; // SPECULATION: May be deleted
        
        size RAMsize, ROMsize, programSize;
        Memory rom;
        Memory ram;
        
        bool LoadFileToROM(std::string filePath);
        CPUes GetExecutionState();
        CPUes Execute();
        CPUes ExecuteIgnoreCPUExecState();
        void SetExecutionState(CPUes ExecState, bool isFatal); // TO FIX OR REDO
        
        virtual void ExecuteInstruction() { SetExecutionState(CPUes::INVALID_CPU, true); }; // FUNCTION PLACEHOLDER, CHANGES FROM CPU TO CPU.
        
        virtual void Init() {} // FUNCTION PLACEHOLDER, CHANGES FROM CPU TO CPU
        
        virtual void IncreasePC(size amount) {} // FUNCTION PLACEHOLDER, CHANGES FROM CPU TO CPU.
        virtual void SetPC (size address) {} // FUNCTION PLACEHOLDER, CHANGES FROM CPU TO CPU.

        virtual void PrintCPUData() {} // FUNCTION PLACEHOLDER, CHANGES FROM CPU TO CPU
        virtual void PrintRegisters() {} // FUNCTION PLACEHOLDER, CHANGES FROM CPU TO CPU
};