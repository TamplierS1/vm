#include "instructions.h"

void add(uint16_t instruction)
{
    uint16_t dr = instruction >> 9 & 0x7;
    uint16_t sr1 = instruction >> 6 & 0x7;
    uint16_t mode = instruction >> 5 & 0x1;

    uint16_t result = 0;
    // immediate mode
    if (mode)
    {
        // extract value from instruction
        uint16_t value = sign_extend(instruction & (0xFFFF >> 11), 5);
        result = reg_read(sr1) + value;
    }
    else
    {
        // TODO: test this case
        uint16_t sr2 = instruction & (0xFFFF >> 13);
        result = reg_read(sr1) + reg_read(sr2);
    }
    reg_write(dr, result);

    update_flags(reg_read(dr));
}

void load_indirect(uint16_t instruction)
{
    uint16_t dr = instruction >> 9 & 0x7;
    uint16_t pc_offset9 = sign_extend(instruction & (0xFFFF >> 7), 9);
    uint16_t value_address = mem_read(reg_read(Registers::PC) + pc_offset9);
    uint16_t value = mem_read(value_address);
    reg_write(dr, value);

    update_flags(reg_read(dr));
}

void and_bitwise(uint16_t instruction)
{
    uint16_t dr = instruction >> 9 & 0x7;
    uint16_t sr1 = instruction >> 6 & 0x7;
    uint16_t mode = instruction >> 5 & 0x1;

    uint16_t result = 0;
    // immediate mode
    if (mode)
    {
        // extract value from instruction
        uint16_t value = sign_extend(instruction & (0xFFFF >> 11), 5);
        result = reg_read(sr1) & value;
    }
    else
    {
        // TODO: test this case
        uint16_t sr2 = instruction & (0xFFFF >> 13);
        result = reg_read(sr1) & reg_read(sr2);
    }
    reg_write(dr, result);

    update_flags(reg_read(dr));
}