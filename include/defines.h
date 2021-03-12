#ifndef DEFINES_H
#define DEFINES_H

enum Registers
{
    R0 = 0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    PC,  // program counter
    COND,
    COUNT
};

enum Opcodes
{
    BR = 0,
    ADD,
    LD,
    ST,
    JSR,
    AND,
    LDR,
    STR,
    RTI,
    NOT,
    LDI,
    STI,
    JMP,
    RES,
    LEA,
    TRAP
};

enum ConditionFlags
{
    // each condition flag represents its own bit
    POS = 1,
    ZRO = 2,
    NEG = 4
};

const uint16_t g_pc_start = 0x3000;

#endif  // DEFINES_H
