#include "4004.hpp"
#include "ISA.hpp"

void Intel_4004::PrintRegisters() {
    std::cout << "Intel 4004 Instance Data: \n" <<
    "\nCPU Can Execute Flag: " << canExecute <<
    "\nCPU Execution State: " << executionState <<
    "\n\nProgram Counter: " << registers12[PC] << 
    "\nStack Pointer 1: " << registers12[SP1] << 
    "\nStack Pointer 2: " << registers12[SP2] <<
    "\nStack Pointer 3: " << registers12[SP3] <<
    "\nRegisters: " <<
    "\n     +------------+---------+---------+" <<
    "\n     |  Register  |Dec Value|Hex Value|" <<
    "\n     +------------+---------+---------+";
    printf("\n     |Accumulator |    %2d   |     %01x   |", registers4[A], registers4[A]);
    for (u8 i = 0; i < 16; i++)
        printf("\n     |    R%-2d     |    %2d   |     %01x   |", i, registers4[i], registers4[i]);
    std::cout << "\n     +------------+---------+---------+\n\n";
}


void Intel_4004::PrintCPUData() {
    std::cout << "Intel 4004 Instance Data: \n" <<
    "\nCPU Can Execute Flag: " << canExecute <<
    "\nCPU Execution State: " << executionState <<
    "\n\nProgram Counter: " << registers12[PC] << 
    "\nStack Pointer 1: " << registers12[SP1] << 
    "\nStack Pointer 2: " << registers12[SP2] <<
    "\nStack Pointer 3: " << registers12[SP3] <<
    "\nRegisters: " <<
    "\n     +------------+---------+---------+" <<
    "\n     |  Register  |Dec Value|Hex Value|" <<
    "\n     +------------+---------+---------+";
    printf("\n     |Accumulator |    %2d   |     %01x   |", registers4[A], registers4[A]);
    for (u8 i = 0; i < 16; i++)
        printf("\n     |    R%-2d     |    %2d   |     %01x   |", i, registers4[i], registers4[i]);
    std::cout << "\n     +------------+---------+---------+\n\n";
    
    std::cout << "ROM Contents:" <<
    "\n     +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+" <<
    "\n     | CH0 | CH1 | CH2 | CH3 | CH4 | CH5 | CH6 | CH7 | CH8 | CH9 | CH10| CH11| CH12| CH13| CH14| CH15|" <<
    "\n     +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+";
    for (size i = 0; i < ROMsize; i++) {
        if (i % 16 == 0) printf("\n     |");
        printf("  %02x |", rom[i]);
    }
    std::cout << "\n     +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+\n\n";
    //*/
    std::cout <<
    "RAM Contents:" <<
    "\n     +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+" <<
    "\n     | CH0 | CH1 | CH2 | CH3 | CH4 | CH5 | CH6 | CH7 | CH8 | CH9 | CH10| CH11| CH12| CH13| CH14| CH15| SC0 | SC1 | SC2 | SC4 |" <<
    "\n     +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+";
    size j = 0;
    for (size i = 0; i < RAMsize - 256; i += 16) {
        printf("\n     |");
        for (u8 k = 0; k < 16; k++) 
            printf("  %x  |", ram4[i + k]);
        for (u8 k = 0; k < 4; k++)
            printf("  %x  |", ram4[1024 + j + k]);
        j += 4;
    }
    
    std::cout <<
    "\n     +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+" <<
    "\n\nRAM Output Ports: " <<
    "\n     +--------+---------+" <<
    "\n     |  Port  |  Value  |" <<
    "\n     +--------+---------+";
    for (size i = 0; i < 32; i++) {
        printf("\n     |   %3lld  |    %3d  |", i, registers4[RAOP0 + i]);
    }
    std::cout << "\n     +--------+---------+" <<
    "\n\nROM I/O Ports: " <<
    "\n     +--------+---------+" <<
    "\n     |  Port  |  Value  |"
    "\n     +--------+---------+";
    for (size i = 0; i < 16; i++) {
        printf("\n     |   %3lld  |    %3d  |", i, registers4[RP0 + i]);
    }
    std::cout << "\n     +--------+---------+" <<
    std::endl;
}

void Intel_4004::IncreasePC(size amount) {
    if (executionState == REACHED_EOF || executionState == REACHED_ROM_END) { registers12[PC] = 0; }
    if (registers12[PC] > programSize) { SetExecutionState(REACHED_EOF, true); registers12[PC] = _4k - 1; }
    if (registers12[PC] > ROMsize) { SetExecutionState(REACHED_ROM_END, true); registers12[PC] = 0; };
    registers12[PC] += amount;
}

void Intel_4004::SetPC(size address) {
    registers12[PC] = address;
    if(registers12[PC] >= ROMsize) { registers12[PC] = 0; };
}

void Intel_4004::Init() {
    registers12[PC] = 0;
    zeroArray(ram4, RAMsize);
    zeroArray(registers12, ARRAY_SIZE(registers12));
    zeroArray(registers4, ARRAY_SIZE(registers4));
    zeroArray(portSettings, ARRAY_SIZE(portSettings));
    flags[C] = false;
    
    currentStackRegister = 0;
    selectedRAMDataBank = 0;
    selectedSRCAddress = 0;
    
    defaultOutputPortReadState = false;
    
    
    currentByte = 0;
    currentByte2W = 0;
    
    upperNibble = 0;
    lowerNibble = 0;
    upperNibble2W = 0;
    lowerNibble2W = 0;
    
    SetExecutionState(CPUes::INITIALIZED, false);
}

// Creates an Intel 4004 Instance with the maximum supported RAM and ROM sizes.
Intel_4004::Intel_4004() {
    RAMsize = 1280;
    ROMsize = _4k;
    ram4.resize(RAMsize);
    rom.resize(ROMsize);
    Init();
}

// WILL POSSIBLY BE DELETED DUE TO ARCHITECTURAL LIMITATIONS
// HANDLING OF SMALLER ROM AND RAM SIZES NEEDS TO BE CODED
// Creates an Intel 4004 Instance with custom RAM and ROM sizes. NOT RECOMMENDED
Intel_4004::Intel_4004(size romSize, size ramSize) {
    if (romSize > MAX_ROM_SIZE_4004) {
        SetExecutionState(CPUes::ALLOCATED_ROM_UP_TO_ARCH_LIMITS, false);
        ramSize = MAX_ROM_SIZE_4004;
    }
    ROMsize = romSize;
    if (ramSize > MAX_RAM_SIZE_4004) {
        SetExecutionState(CPUes::ALLOCATED_RAM_UP_TO_ARCH_LIMITS, false);
        ramSize = MAX_RAM_SIZE_4004;
    }
    RAMsize = ramSize;
    rom.resize(ROMsize);
    ram4.resize(RAMsize);
    Init();
}

Intel_4004::~Intel_4004() {
    ram4.clear();
    rom.clear();
}

bool Intel_4004::setROMPortMode(u4 IOPortNum, b4 mode0, b4 mode1, b4 mode2, b4 mode3) {
    if (IOPortNum > 15) return true;
    portSettings[IOPortNum * 4] = mode0;
    portSettings[IOPortNum * 4 + 1] = mode1;
    portSettings[IOPortNum * 4 + 2] = mode2;
    portSettings[IOPortNum * 4 + 3] = mode3;
    return false;
}

// May need to be rewritten.
// Returns 0 if operation was successful or 1 if not or a port was an Output.
bool Intel_4004::setROMPortBitStatus(u4 IOPortNum, b4 status) {
    bool returnCode = false;
    if (IOPortNum > 15) return true;
    b4 inputData = 0;
    for (u4 i = 0; i < 4; i++) {
        if (portSettings[IOPortNum * 4 + i] != NON_INVERTING_OUTPUT &&
            portSettings[IOPortNum * 4 + i] != INVERTING_OUTPUT &&
            portSettings[IOPortNum * 4 + i] != MIXED) {
                returnCode = true;
                inputData |= registers4[RP0 + IOPortNum] & (0b0001 << i);
        } else
            inputData |= status & (0b0001 << i);
    }
    registers4[RP0 + IOPortNum] = inputData;
    return returnCode;
}

// Returns 0 if Selected Port is invalid.
b4 Intel_4004::getROMPortBitStatus(u4 IOPortNum) {
    if (IOPortNum > 15 ) return 0;
    b4 output = 0;
    for (u4 i = 4; i > 0; i--) {
        if (portSettings[IOPortNum * 4 + i] != NON_INVERTING_INPUT &&
            portSettings[IOPortNum * 4 + i] != INVERTING_INPUT &&
            portSettings[IOPortNum * 4 + i] != MIXED)
            output = output << 1 | defaultOutputPortReadState;
        else
            output = output << 1 | (registers4[P0 + IOPortNum] && 1 << i);
    }
    return output;
};

void Intel_4004::setRAMOutputPort(b4 data) {
    registers4[RAOP0 + selectedRAMDataBank * 4 + (selectedSRCAddress >> 6)] = data;
};

// Returns 0 if the arguments are invalid.
b4 Intel_4004::getRAMOutputPort(u4 RAMBank, u4 OPortNum) {
    if (OPortNum > 3 || RAMBank > 7) return 0;
    return registers4[RAOP0 + RAMBank * 4 + OPortNum];
};

