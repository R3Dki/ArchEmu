#pragma once
/*
*        Intel 4004 w/o Memory Emulation ISA
*   upperNibble   lowerNibble
*            \/   \/
*   Byte:   1001 0110
*
*/
// Memory Limitations:
#define MAX_ROM_SIZE_4004 _4k
#define MAX_RAM_SIZE_4004 1280

// Upper Nibbles for 1 Word Instructions 
#define NOP 0b0000
#define JCN 0b0001
#define FIM 0b0010 //RRR0 WARNING: This OpCode is also used for the SRC instruction.
#define SRC 0b0001 //RRR1 WARNING: This is a Lower Nibble Bitmask
#define FIN 0b0011 //RRR0 WARNING: This OpCode is also used for the JIN instruction.
#define JIN 0b0001 //RRR1 WARNING: This is a Lower Nibble Bitmask
#define JUN 0b0100
#define JMS 0b0101
#define INC 0b0110
#define ISZ 0b0111
#define ADD 0b1000
#define SUB 0b1001
#define LD  0b1010
#define XCH 0b1011
#define BBL 0b1100
#define LDM 0b1101

#define E_OPCODES 0b1110
// Lower Nibbles for 2 Word Instructions 
#define WRM 0b0000
#define WMP 0b0001
#define WRR 0b0010
#define WPM 0b0011
#define WR0 0b0100
#define WR1 0b0101
#define WR2 0b0110
#define WR3 0b0111
#define SBM 0b1000
#define RDM 0b1001
#define RDR 0b1010
#define ADM 0b1011
#define RD0 0b1100
#define RD1 0b1101
#define RD2 0b1110
#define RD3 0b1111

#define F_OPCODES 0b1111
// Lower Nibbles for 2 Word Instructions
#define CLB 0b0000
#define CLC 0b0001
#define IAC 0b0010
#define CMC 0b0011
#define CMA 0b0100
#define RAL 0b0101
#define RAR 0b0110
#define TCC 0b0111
#define DAC 0b1000
#define TCS 0b1001
#define STC 0b1010
#define DAA 0b1011
#define KBP 0b1100
#define DCL 0b1101
