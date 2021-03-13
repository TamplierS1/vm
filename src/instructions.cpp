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
        uint16_t sr2 = instruction & (0xFFFF >> 13);
        result = reg_read(sr1) & reg_read(sr2);
    }
    reg_write(dr, result);

    update_flags(reg_read(dr));
}

void conditional_branch(uint16_t instruction)
{
    // condition flags
    uint16_t pc_offset9 = instruction & (0xFFFF >> 7);
    uint16_t flags = instruction >> 9 & 0x7;

    if (flags & reg_read(Registers::COND))
    {
        uint16_t pc = reg_read(Registers::PC);
        reg_write(Registers::PC, pc + pc_offset9);
    }
}

void jump(uint16_t instruction)
{
    uint16_t base_r = instruction >> 6 & 0x7;

    reg_write(Registers::PC, reg_read(base_r));
}

void jump_subroutine(uint16_t instruction)
{
    uint16_t mode = instruction & (1 << 11);
    // save the address of the calling routine
    reg_write(Registers::R7, reg_read(Registers::PC));

    // immediate mode
    if (mode)
    {
        uint16_t pc = reg_read(Registers::PC);
        uint16_t pc_offset11 = instruction & (0xFFFF >> 5);
        reg_write(Registers::PC, pc + pc_offset11);
    }
    else
    {
        uint16_t base_r = instruction >> 6 & 0x7;
        reg_write(Registers::PC, reg_read(base_r));
    }
}
