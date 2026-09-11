#pragma once
#include "../../Common/cpu_required.hpp"

class ARM_Generic32 {
private:
    IPS instructionsPerSecond = 0;
    size RAMsize = 0;
    size ROMsize = 0;
    Memory ram;
    Memory rom;
    Flag flags[4];
    Register32 registers[15];

    inline void SetPC (u64 address);
    inline void IncrementPC(u8 incrementAmount);
    inline void ExecuteInstruction (byte currentByte);
    void CPUErr(u8 errorCode); // TO FIX OR REDO

    public:
    enum aliasReg32 {
        R0,  // Register 0
        R1,  // Register 1
        R2,  // Register 2
        R3,  // Register 3
        R4,  // Register 4
        R5,  // Register 5
        R6,  // Register 6
        R7,  // Register 7
        R8,  // Register 8
        R9,  // Register 9
        R10, // Register 10
        R11, // Register 11
        R12, // Register 12
        R13, // Stack Pointer
        R14, // Link Register
        R15, // Program Counter
        SP = 13,
        LR,
        PC
    };

    enum aliasFlags {
        N, // Negative Flag
        C, // Carry Flag
        Z, // Zero Flag
        V  // Overflow Flag
    };
    ARM_Generic32();
    ARM_Generic32(size ROMsize, size RAMsize);
    ~ARM_Generic32();
    void execute();

};


ARM_Generic32::ARM_Generic32() {
    RAMsize = _16M;
    ROMsize = _128M;
    ram.resize(RAMsize);
    rom.resize(ROMsize);
}

ARM_Generic32::ARM_Generic32(size romSize, size ramSize) {
    RAMsize = ramSize;
    ROMsize = romSize;
    ram.resize(RAMsize);
    rom.resize(ROMsize);
}

ARM_Generic32::~ARM_Generic32() {
}
