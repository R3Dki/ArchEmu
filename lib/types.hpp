#pragma once

// ArchEmu Specific Types Library

// Types Library Compiled Settings
#define DISABLE_BITWISE_TYPES 0             // Disables types that are used for bitwise operations(byte, word, dword, qword, block, page).
#define DISABLE_SPECIFIC_TYPES 0            // Disables types that are used for specific applications such as timestamps, sizes and strings.
#define DISABLE_ERRORCODE_TYPE 1            // Disables err(errorcode) type used for function execution result. WARNING: Will cause errors in some libraries if disabled. 
#define DISABLE_CHECKED_4BIT_STRUCT 1
#define PAGE_SIZE 512                       // Page type size specifier.
// Types Library Compiled Settings

#include <vector>

#define TYPES_LIB_APPLICATION "ArchEmu Specific Types Library"
#define TYPES_LIB_VERSION_MAJOR 1
#define TYPES_LIB_VERSION_MINOR 0
#define TYPES_LIB_VERSION_PATCH 1


#if !DISABLE_BITWISE_TYPES
typedef unsigned char nibble;   // UNSAFE: The real size of the nibble type is 8 bits, not 4, which may lead to undefined behaviour if used the wrong way.
#if !DISABLE_BYTE_TYPE
    typedef unsigned char byte;
#endif
typedef unsigned short word;
typedef unsigned int dword;
typedef unsigned long long qword; // WARNING: May not be 64 bit long in some architectures, needs to be reviewed.
typedef word bitgroup;          // 16 bits wide group of bits.
typedef byte block[];           // EXPERIMENTAL: Use with caution. Should be used whenever a byte sequence variable is needed.
typedef byte page[PAGE_SIZE];   // EXPERIMENTAL: Use with caution. Should be used whenever a large and specificly sized byte sequence variable is needed.
#endif // !DISABLE_SPECIFIC_TYPES
// Bit Types
#if DISABLE_CHECKED_4BIT_STRUCT
    typedef unsigned char b4;
#endif
typedef unsigned char b8;
typedef unsigned short b12; // WARNING: This is a renamed 16 bits variable.
typedef unsigned short b16;
typedef unsigned int b32;
typedef unsigned long long b64; // WARNING: May not be 64 bit long in some architectures, needs to be reviewed.

#if !DISABLE_CHECKED_4BIT_STRUCT
    class Register4 {
        private:
        public:
        u4 value;
        Register4(u4 x);
        ~Register4();
    };
    
    Register4::Register4() { value = 0; }

    Register4::Register4(u4 x) { value = x; }
    
    Register4::~Register4();
    Register4 operator+ (Register4 const& r1, Register4 const& r2){
        u4 tmp = r1.value + r2.value;
        tmp = (tmp > 0b1111) ? tmp % 16 : tmp;
        return Register4(tmp);
    }
    
#endif
// Bit Types

#if DISABLE_CHECKED_4BIT_STRUCT
    typedef unsigned char u4;
#endif
typedef unsigned char u8;
typedef unsigned short u12; // WARNING: This is a renamed 16 bits variable.
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64; // WARNING: May not be 64 bit long in some architectures, needs to be reviewed.
    
#if DISABLE_CHECKED_4BIT_STRUCT
    typedef signed char i4;
#endif
typedef signed char i8;
typedef signed short i12; // WARNING: This is a renamed 16 bits variable.
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64; // WARNING: May not be 64 bit long in some architectures, needs to be reviewed.

typedef float f32;
typedef double f64;
typedef long double f128; // WARNING: May not be 128 bit long in some architectures, needs to be reviewed.

#if !DISABLE_SPECIFIC_TYPES
// typedef unsigned short wchar;
typedef unsigned long long size;
typedef unsigned long long timestamp;
typedef signed long long unix_time;
#endif

// CPU Execution State Type
enum CPUes {
    SUCCESS,                            // Instruction Cycle Executed Successfully
    INITIALIZED,                        // CPU Initialized
    READY,                              // Ready to execute the program loaded in ROM
    ALLOCATED_RAM_UP_TO_ARCH_LIMITS,    // Allocated the maximum amount of RAM allowed by the Architecture.
    ALLOCATED_ROM_UP_TO_ARCH_LIMITS,    // Allocated the maximum amount of ROM allowed by the Architecture.
    INVALID_OPCODE,                     // Invalid OpCode in ROM
    REACHED_EOF,                        // Reached ROM EOF
    REACHED_ROM_END,                    // Reached End of ROM
    REACHED_RAM_END,                    // Reached End of RAM
    INVALID_ROM_ADDR,                   // Received Invalid ROM Address
    INVALID_RAM_ADDR,                   // Received Invalid RAM Address
    OUT_OF_MEMORY,                      // Out of RAM
    OUT_OF_STORAGE,                     // Out of ROM
    INVALID_CPU                         // A CPU Class Instance has been called, by design it shouldn't be possible.
};

#if !DISABLE_ERRORCODE_TYPE
    typedef enum errorcode {
        SUCCESS, // Successful Execution
        FAILURE, // Generic Failure
        UNKNOWN, // Unknown Failure
        INVALID_NIBBLE, // Given Nibble is Invalid
        UNEXPECTED_DTS, // Unexpected Data Type Specifier
        NO_ARGS_TO_PARSE, // No Arguments to Parse
        EXPECTED_ARGS_MISMATCH // Received Args do not match Expected ones
    } err;
#endif

// --------------------MPU Types-------------------------
typedef unsigned long long IPS; // Instruction Per Second Property Type
typedef std::vector<byte> Memory; // Memory Data Type organized in bytes
typedef bool pinState; // CPU Pin State Type    WILL PROBABLY BE DELETED
typedef bool Flag; // CPU Flag Data Type
typedef b4 Register4; // 4 Bit Wide Register
typedef b8 Register8; // 8 Bit Wide Register
typedef b16 Register12; // 12 Bit Wide Register WARNING: This is a renamed 16 bits register.
typedef b16 Register16; // 16 Bit Wide Register
typedef b32 Register32; // 32 Bit Wide Register
typedef b64 Register64; // 64 Bit Wide Register
union MultiWidthRegister8 {b8 b; b4 n;}; // b: BYTE | n: NIBBLE
union MultiWidthRegister16 {b16 w; b8 b; b4 n;}; // w: WORD | b: BYTE | n: NIBBLE
union MultiWidthRegister32 {b32 d; b16 w; b8 b; b4 n;}; // d: DWORD | w: WORD | b: BYTE | n: NIBBLE
union MultiWidthRegister64 {b64 q; b32 d; b16 w; b8 b; b4 n;}; //q: QWORD | d: DWORD | w: WORD | b: BYTE | n: NIBBLE

#define _1k 1024
#define _2k 2048
#define _4k 4096
#define _8k 8192
#define _16k 16384
#define _32k 32768
#define _64k 65536
#define _128k 131072
#define _256k 262144
#define _512k 524288
#define _1M 1048576
#define _2M 2097152
#define _4M 4194304
#define _8M 8388608
#define _16M 16777216
#define _32M 33554432
#define _64M 67108864
#define _128M 134217728
#define _256M 268435456
#define _512M 536870912
#define _1G 1073741824
#define _2G 2147483648
#define _4G 4294967296
// --------------------MPU Types-------------------------


// Returns the size of the array it receives as argument.
// WARNING: Do not use this macro on pointers to arrays since it'll always return the size of the pointer. (This warning may need to be fixed)
#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
