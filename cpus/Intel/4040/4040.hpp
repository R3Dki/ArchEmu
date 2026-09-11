#pragma once
#include "../../Common/cpu_required.hpp"

// Intel 4040
// When reading a ROM Port set as Output it will return its current state.

class Intel_4040 : public CPU{
    #if !CPU_DEBUG_MODE
    private:
    #else
    public:
    #endif
        /*
        Emulated RAM Structure:
            +---------------+-------+
            |   Characters  |St. Chr|
            +-+-+-+-+-+-+-+-+-+-+-+-+
            |C|C|C|C|C|C|C|C|S|S|S|S|
            +-+-+-+-+-+-+-+-+-+-+-+-+
        
        Effective RAM Structure:
            +---------------+
            |   Characters  |
            +-+-+-+-+-+-+-+-+
            |C|C|C|C|C|C|C|C|
            +-+-+-+-+-+-+-+-+
            |C|C|C|C|C|C|C|C|
            +-+-+-+-+-+-+-+-+

            Status Characters Memory Region starts at address: 1024
            +-+-+-+-+
            |St. Chr|
            +-+-+-+-+
            |S|S|S|S|
            +-+-+-+-+
            |S|S|S|S|
            +-+-+-+-+
        */
        std::vector<nibble> ram4;
        Flag flags[3];
        b4 currentStackRegister,
            selectedRAMDataBank;

        // Port Settings Structure:    
        //    +--+--+--+--+--+
        //    |Ad|00|01|02|03|
        //    +--+--+--+--+--+    
        //    |00|PS|PS|PS|PS|
        //    +--+--+--+--+--+    
        //    |04|PS|PS|PS|PS|
        //    +--+--+--+--+--+
        //
        // Port Settings Options:
        //    0b0000 0 - Mixed / Unset
        //    0b0001 1 - Non-inverting output
        //    0b0010 2 - Inverting output
        //    0b0011 3 - Non-inverting input
        //    0b0100 4 - Inverting input
        b4 portSettings[64];
        bool defaultOutputPortReadState;
        b8 selectedSRCAddress;
        Register4 registers4[74];
        Register12 registers12[8];

        bool testPinState = false;
        byte currentByte,
             currentByte2W;
        b4 upperNibble,
           lowerNibble,
           upperNibble2W,
           lowerNibble2W;
            
            void SetPC (u64 address) override; //           Probably Temporary
            void IncreasePC (u64 amount) override;
            void Init() override;
            
            inline void PushStack(b12 address);
            inline b12 PopStack();
        
            inline b12 CalculateEffectiveRAMAddress();
            inline b12 CalculateEffectiveRAMStatusAddress(b4 statusCharNumber);
            inline void ReadNextROMByte();
            inline void ReadNextROMByte2W();
            inline b4 ReadROMNibble(b12 address, bool lowerNibble);
            inline b8 ReadROMByte(b12 address);
            void setRAMOutputPort(b4 data);


        public:
        enum IOPortModes {
            MIXED = 0b000,
            NON_INVERTING_OUTPUT = 0b001,
            INVERTING_OUTPUT = 0b010,
            NON_INVERTING_INPUT = 0b011,
            INVERTING_INPUT = 0b100
        };
        
        enum aliasReg4 {
            R0,     // Index Register 0
            R1,     // Index Register 1
            R2,     // Index Register 2
            R3,     // Index Register 3
            R4,     // Index Register 4
            R5,     // Index Register 5
            R6,     // Index Register 6
            R7,     // Index Register 7
            R8,     // Index Register 8
            R9,     // Index Register 9
            R10,    // Index Register 10
            R11,    // Index Register 11
            R12,    // Index Register 12
            R13,    // Index Register 13
            R14,    // Index Register 14
            R15,    // Index Register 15
            RR0,    // Index Register B1 0
            RR1,    // Index Register B1 1
            RR2,    // Index Register B1 2
            RR3,    // Index Register B1 3
            RR4,    // Index Register B1 4
            RR5,    // Index Register B1 5
            RR6,    // Index Register B1 6
            RR7,    // Index Register B1 7
            RP0,    // ROM Port 0
            RP1,    // ROM Port 1
            RP2,    // ROM Port 2
            RP3,    // ROM Port 3
            RP4,    // ROM Port 4
            RP5,    // ROM Port 5
            RP6,    // ROM Port 6
            RP7,    // ROM Port 7
            RP8,    // ROM Port 8
            RP9,    // ROM Port 9
            RP10,   // ROM Port 10
            RP11,   // ROM Port 11
            RP12,   // ROM Port 12
            RP13,   // ROM Port 13
            RP14,   // ROM Port 14
            RP15,   // ROM Port 15,
            RAOP0,  // RAM Output Port
            RAOP1,  // RAM Output Port
            RAOP2,  // RAM Output Port
            RAOP3,  // RAM Output Port
            RAOP4,  // RAM Output Port
            RAOP5,  // RAM Output Port
            RAOP6,  // RAM Output Port
            RAOP7,  // RAM Output Port
            RAOP8,  // RAM Output Port
            RAOP9,  // RAM Output Port
            RAOP10, // RAM Output Port
            RAOP11, // RAM Output Port
            RAOP12, // RAM Output Port
            RAOP13, // RAM Output Port
            RAOP14, // RAM Output Port
            RAOP15, // RAM Output Port
            RAOP16, // RAM Output Port
            RAOP17, // RAM Output Port
            RAOP18, // RAM Output Port
            RAOP19, // RAM Output Port
            RAOP20, // RAM Output Port
            RAOP21, // RAM Output Port
            RAOP22, // RAM Output Port
            RAOP23, // RAM Output Port
            RAOP24, // RAM Output Port
            RAOP25, // RAM Output Port
            RAOP26, // RAM Output Port
            RAOP27, // RAM Output Port
            RAOP28, // RAM Output Port
            RAOP29, // RAM Output Port
            RAOP30, // RAM Output Port
            RAOP31, // RAM Output Port
            CR,      // Command Register
            A       // Accumulator
        };
        
        enum aliasReg12 {
            SP1, // Call Level 1 / Stack Pointer 1
            SP2, // Call Level 2 / Stack Pointer 2
            SP3, // Call Level 3 / Stack Pointer 3
            SP4, // Call Level 4 / Stack Pointer 4
            SP5, // Call Level 5 / Stack Pointer 5
            SP6, // Call Level 6 / Stack Pointer 6
            SP7, // Call Level 7 / Stack Pointer 7
            PC   // Program Counter
        };
        
        enum aliasFlags {
            C,  // Carry Flag
            SB, // Selected Ram Bank (0-1)
            IE  // Interrupts Enabled
        };
        
        enum aliasRegPairs { 
            P0 = 0,
            P1 = 2,
            P2 = 4,
            P3 = 6,
            P4 = 8,
            P5 = 10,
            P6 = 12,
            P7 = 14
        };
        
        
        Intel_4040();
        Intel_4040(size ROMsize, size RAMsize);
        ~Intel_4040();
        void PrintCPUData() override;
        void PrintRegisters() override;
        
        void ExecuteInstruction() override;
        
        bool setROMPortMode(u4 IOPortNum, b4 mode0, b4 mode1, b4 mode2, b4 mode3);
        bool setROMPortBitStatus(u4 IOPortNum, b4 status);
        b4 getROMPortBitStatus(u4 IOPortNum);
        b4 getRAMOutputPort(u4 RAMBank, u4 OPortNum);
};
