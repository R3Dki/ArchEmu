#include "ISA.hpp"
#include "4004.hpp"

inline b12 Intel_4004::CalculateEffectiveRAMAddress() {
    b4 dataRAMChip = selectedSRCAddress >> 6;
    b4 RAMRegister = (selectedSRCAddress >> 4) & 0b00000011;
    b4 selectedCharacter = selectedSRCAddress & 0b00001111;
    return ((selectedRAMDataBank * 256) + (dataRAMChip * 64) + (RAMRegister * 16) + selectedCharacter);
}

inline b12 Intel_4004::CalculateEffectiveRAMStatusAddress(b4 statusCharNumber) {
    b4 dataRAMChip = selectedSRCAddress >> 6;
    b4 RAMRegister = (selectedSRCAddress >> 4) & 0b00000011;
    return (1024 + (selectedRAMDataBank * 4) + (dataRAMChip * 4) + (RAMRegister * 4) + statusCharNumber);
}

inline void Intel_4004::ReadNextROMByte() {
    currentByte = rom[registers12[PC]];
    upperNibble = currentByte >> 4;
    lowerNibble = currentByte & 0b00001111;
    IncreasePC(1);
}

inline void Intel_4004::ReadNextROMByte2W() {
    currentByte2W = rom[registers12[PC]];
    upperNibble2W = currentByte2W >> 4;
    lowerNibble2W = currentByte2W & 0b00001111;
    //IncreasePC(1);
}

inline b4 Intel_4004::ReadROMNibble(b12 address, bool lowerNibble) {
    if (address >= ROMsize) { SetExecutionState(INVALID_ROM_ADDR, true); return 0; }
    if (lowerNibble)
        return rom[address] & 0b00001111;
    else
        return rom[address] >> 4;
}

inline b8 Intel_4004::ReadROMByte(b12 address) {
    if (address >= ROMsize) {SetExecutionState(INVALID_ROM_ADDR, true); return 0;}
    return rom[address];
}

inline b12 Intel_4004::PopStack() {
    if (currentStackRegister == 0) currentStackRegister = 3;
    currentStackRegister--;
    return registers12[currentStackRegister];
}

inline void Intel_4004::PushStack(b12 address) {
    registers12[currentStackRegister] = address;
    currentStackRegister++;
    if (currentStackRegister > 2) currentStackRegister = 0;
}




void Intel_4004::ExecuteInstruction () {
    executionState = CPUes::SUCCESS;
    ReadNextROMByte();

    switch (upperNibble) {
        case NOP:
            // NOP
        break;
        case JCN:
            {
                ReadNextROMByte2W();
                bool invertCondition = lowerNibble & 0b1000;
                bool isAccumulatorZero = ((lowerNibble & 0b0100) && !registers4[A]);
                bool isCarryOne = ((lowerNibble & 0b0010) && flags[C]);
                bool isTestOne = (lowerNibble & 0b0001) && testPinState;
                if (invertCondition) {
                    isAccumulatorZero = !isAccumulatorZero;
                    isCarryOne = !isCarryOne;
                    isTestOne = !isTestOne;
                }
                if (isAccumulatorZero || isCarryOne || isTestOne) {
                    registers12[PC] &= 0b0000111100000000;
                    registers12[PC] |= registers4[upperNibble2W] << 4;
                    registers12[PC] |= registers4[lowerNibble2W];
                }
            }
        break;
        case FIM: // Executes SRC if the last bit is 1
            if (lowerNibble & SRC) {
                lowerNibble >>= 1;
                selectedSRCAddress &= 0b00000000;
                selectedSRCAddress |= registers4[lowerNibble] << 4;
                selectedSRCAddress |= registers4[lowerNibble + 1];
            } else {
                ReadNextROMByte2W();
                lowerNibble >>= 1;
                registers4[lowerNibble] = upperNibble2W;
                registers4[lowerNibble + 1] = lowerNibble2W;
            }
        break;
        case FIN: // Executes JIN if the last bit is 1
            if (lowerNibble & JIN) {
                lowerNibble >>= 1;
                registers12[PC] &= 0b0000111100000000;
                registers12[PC] |= registers4[lowerNibble] << 4;
                registers12[PC] |= registers4[lowerNibble + 1];
            } else {
                b12 calculatedAddress = registers12[PC] & 0b0000111100000000 | (registers4[P0] << 4) | registers4[P0 + 1];
                lowerNibble >>= 1;
                registers4[lowerNibble] = ReadROMNibble(calculatedAddress, false);
                registers4[lowerNibble + 1] = ReadROMNibble(calculatedAddress, true);
            }
        break;
        case JUN:
            ReadNextROMByte2W();
            registers12[PC] &= 0b0000000000000000;
            registers12[PC] |= lowerNibble << 8;
            registers12[PC] |= upperNibble2W << 4;
            registers12[PC] |= lowerNibble2W;
        break;
        case JMS:
            ReadNextROMByte2W();
            PushStack(registers12[PC] + 1);
            registers12[PC] &= 0b0000000000000000;
            registers12[PC] |= lowerNibble << 8;
            registers12[PC] |= upperNibble2W << 4;
            registers12[PC] |= lowerNibble2W;
        break;
        case INC:
            (++registers4[lowerNibble] <= 0b1111 ) ? 0 : registers4[lowerNibble] = 0;
        break;
        case ISZ:
            ReadNextROMByte2W();
            IncreasePC(1);
            (++registers4[lowerNibble] <= 0b1111 ) ? 0 : registers4[lowerNibble] = 0;
            if (registers4[lowerNibble]) {
                registers12[PC] &= 0b0000111100000000;
                registers12[PC] |= upperNibble2W << 4;
                registers12[PC] |= lowerNibble2W;
            }
        break;
        case ADD: // Carry Flag may be wrong
            registers4[A] += registers4[lowerNibble] + ((flags[C]) ? 1 : 0);
            if (registers4[A] > 0b1111) {
                registers4[A] %= 16;
                flags[C] = true;
            } else 
                flags[C] = false;
            //registers4[A] &= 0b1111;
        break;
        case SUB: // Carry Flag may be wrong
            registers4[A] -= registers4[lowerNibble] + ((flags[C]) ? 1 : 0);
            if (registers4[A] > 0b1111 ) {
                registers4[A] = registers4[A] - 251;
                flags[C] = false;
            } else
                flags[C] = true;
            registers4[A] &= 0b1111;
        break;
        case LD:
            registers4[A] = registers4[lowerNibble];
        break;
        case XCH:
            {
                b4 tmp = registers4[A];
                registers4[A] = registers4[lowerNibble];
                registers4[lowerNibble] = tmp;
            }
        break;
        case BBL:
            registers12[PC] = PopStack();
            registers4[A] = lowerNibble;
        break;
        case LDM:
            registers4[A] = lowerNibble;
        break;

        case E_OPCODES:
            //IncreasePC(1);
            switch (lowerNibble) {
                case WRM:
                    ram4[CalculateEffectiveRAMAddress()] = registers4[A];
                break;
                case WMP:
                    setRAMOutputPort(registers4[A]);
                break;
                case WRR:
                    setROMPortBitStatus(selectedSRCAddress >> 4, registers4[A]);
                break;
                case WPM:
                    ram4[CalculateEffectiveRAMAddress()] = registers4[A];
                break;
                case WR0:
                    ram4[CalculateEffectiveRAMStatusAddress(0)] = registers4[A];
                break;
                case WR1:
                    ram4[CalculateEffectiveRAMStatusAddress(1)] = registers4[A];
                break;
                case WR2:
                    ram4[CalculateEffectiveRAMStatusAddress(2)] = registers4[A];
                break;
                case WR3:
                    ram4[CalculateEffectiveRAMStatusAddress(3)] = registers4[A];
                break;
                case SBM:
                    if ((registers4[A] -= ram4[CalculateEffectiveRAMAddress()] + ((flags[C]) ? 1 : 0)) > 0b1111 ) {
                        registers4[A] = registers4[A] - 251;
                        flags[C] = false;
                    } else {
                        flags[C] = true;
                    }
                break;
                case RDM:
                    registers4[A] = ram4[CalculateEffectiveRAMAddress()];
                break;
                case RDR:
                    registers4[A] = getROMPortBitStatus(selectedSRCAddress >> 4);
                break;
                case ADM:
                    if ((registers4[A] += ram4[CalculateEffectiveRAMAddress()] + ((flags[C]) ? 1 : 0)) > 0b1111 ) {
                        registers4[A] = registers4[A] % 16;
                        flags[C] = true;
                    } else {
                        flags[C] = false; }
                break;
                case RD0:
                    registers4[A] = ram4[CalculateEffectiveRAMStatusAddress(0)];
                break;
                case RD1:
                    registers4[A] = ram4[CalculateEffectiveRAMStatusAddress(1)];
                break;
                case RD2:
                    registers4[A] = ram4[CalculateEffectiveRAMStatusAddress(2)];
                break;
                case RD3:
                    registers4[A] = ram4[CalculateEffectiveRAMStatusAddress(3)];
                break;

                default:
                    SetExecutionState(INVALID_OPCODE, false);
                break;
            }
        break;

        case F_OPCODES:
            //IncreasePC(1);
            switch (lowerNibble) {
                case CLB:
                    registers4[A] = 0;
                    flags[C] = 0;
                break;
                case CLC:
                    flags[C] = 0;
                break;
                case IAC:
                    if (++registers4[A] > 0b1111)
                        { registers4[A] = 0; flags[C] = true; }
                    else
                        flags[C] = false; 
                break;
                case CMC:
                    flags[C] = ~flags[C];
                break;
                case CMA:
                    registers4[A] = ~registers4[A];
                    registers4[A] = registers4[A] & 0b00001111;
                break;
                case RAL:
                    {
                        bool tmp = registers4[A] >> 3;
                        registers4[A] <<= 1;
                        registers4[A] |= flags[C];
                        flags[C] = tmp; 
                    }
                break;
                case RAR:
                    {
                        bool tmp = registers4[A] & 0b0001;
                        registers4[A] >>= 1;
                        registers4[A] |= flags[C] << 3;
                        flags[C] = tmp;
                    }
                break;
                case TCC:
                    registers4[A] = flags[C];
                    flags[C] = 0;
                break;
                case DAC:
                    if (--registers4[A] > 0b1111)
                        { registers4[A] = 15; flags[C] = false; }
                    else
                        flags[C] = true;
                break;
                case TCS:
                    registers4[A] = 9 + flags[C];
                    flags[C] = 0;
                break;
                case STC:
                    flags[C] = true;
                break;
                case DAA:
                    if (registers4[A] > 9)
                        if ((registers4[A] += 6) > 0b1111 ) {
                            registers4[A] = registers4[A] % 16;
                            flags[C] = true;
                        }
                break;
                case KBP:
                    if (registers4[A] == 0b0000 || registers4[A] == 0b0001 || registers4[A] == 0b0010) break;
                    if (registers4[A] == 0b0100) { registers4[A] = 0b0011; break; }
                    if (registers4[A] == 0b1000) { registers4[A] = 0b0100; break; }
                    registers4[A] = 0b1111;
                break;
                case DCL:
                    selectedRAMDataBank = registers4[A] & 0b00000111;
                break;
            
                default:
                    SetExecutionState(INVALID_OPCODE, false);
                break;
            }       
        break;
        
        default:
            SetExecutionState(INVALID_OPCODE, false);
        break;
    }
}