#include <catch2/catch.hpp>

#include "instructions.h"
#include "parser.h"

TEST_CASE(" ADD instruction is executed", "[add_instruction]")
{
    SECTION(" in 2 registers mode")
    {
        reg_write(Registers::R1, 2);
        reg_write(Registers::R2, 2);

        std::vector<uint16_t> instr = Parser::get_instance().parse("tests/test_add_2_regs.txt");

        add(instr[0]);

        REQUIRE(reg_read(Registers::R0) == 4);
    }
    SECTION(" in immediate mode")
    {
        reg_write(Registers::R1, 2);

        std::vector<uint16_t> instr = Parser::get_instance().parse("tests/test_add_imm_mode.txt");

        add(instr[0]);

        REQUIRE(reg_read(Registers::R0) == 4);
    }
}

TEST_CASE(" LDI instruction is executed", "[ldi_instruction]")
{
    reg_write(Registers::PC, g_pc_start);
    uint16_t pc = reg_read(Registers::PC);

    mem_write(pc + 5, pc + 6);
    mem_write(pc + 6, 10);

    std::vector<uint16_t> instr = Parser::get_instance().parse("tests/test_ldi.txt");

    load_indirect(instr[0]);

    REQUIRE(reg_read(Registers::R0) == 10);
}

TEST_CASE(" AND instruction is executed", "[and_instruction]")
{
    SECTION(" in 2 registers mode")
    {
        reg_write(Registers::R1, 13);
        reg_write(Registers::R2, 9);

        std::vector<uint16_t> instr = Parser::get_instance().parse("tests/test_and_2_regs.txt");

        and_bitwise(instr[0]);

        REQUIRE(reg_read(Registers::R0) == (13 & 9));
    }
    SECTION(" in immediate mode")
    {
        reg_write(Registers::R1, 6);

        std::vector<uint16_t> instr = Parser::get_instance().parse("tests/test_and_imm_mode.txt");

        and_bitwise(instr[0]);

        REQUIRE(reg_read(Registers::R0) == (7 & 6));
    }
}

TEST_CASE(" BR instruction is executed", "[br_instruction]")
{
    SECTION(" with 'n' passed as a condition flag")
    {
        reg_write(Registers::PC, g_pc_start);

        std::vector<uint16_t> instr = Parser::get_instance().parse("tests/test_branch_n.txt");

        update_flags(-1);

        conditional_branch(instr[0]);

        REQUIRE(reg_read(Registers::PC) == g_pc_start + 3);
    }
    SECTION(" with 'z' passed as a condition flag")
    {
        reg_write(Registers::PC, g_pc_start);

        std::vector<uint16_t> instr = Parser::get_instance().parse("tests/test_branch_z.txt");

        update_flags(0);

        conditional_branch(instr[0]);

        REQUIRE(reg_read(Registers::PC) == g_pc_start + 10);
    }
    SECTION(" with 'p' passed as a condition flag")
    {
        reg_write(Registers::PC, g_pc_start);

        std::vector<uint16_t> instr = Parser::get_instance().parse("tests/test_branch_p.txt");

        update_flags(20);

        conditional_branch(instr[0]);
        REQUIRE(reg_read(Registers::PC) == g_pc_start + 5);
    }
    SECTION(" with 'np' passed as condition flags")
    {
        reg_write(Registers::PC, g_pc_start);

        std::vector<uint16_t> instr = Parser::get_instance().parse("tests/test_branch_np.txt");

        update_flags(-54);

        conditional_branch(instr[0]);
        REQUIRE(reg_read(Registers::PC) == g_pc_start + 5);

        reg_write(Registers::PC, g_pc_start);
        update_flags(23);

        conditional_branch(instr[0]);
        REQUIRE(reg_read(Registers::PC) == g_pc_start + 5);
    }
}

TEST_CASE(" JMP instruction is executed", "[jmp_instruction]")
{
    SECTION(" with valid address")
    {
        reg_write(Registers::PC, g_pc_start);
        reg_write(Registers::R2, g_pc_start + 10);

        std::vector<uint16_t> instr = Parser::get_instance().parse("tests/test_jmp_valid.txt");

        jump(instr[0]);

        REQUIRE(reg_read(Registers::PC) == g_pc_start + 10);
    }
}

TEST_CASE(" JSR instruction is executed", "[jsr_instruction]")
{
    SECTION(" with immediate mode")
    {
        reg_write(Registers::PC, g_pc_start);

        std::vector<uint16_t> instr = Parser::get_instance().parse("tests/test_jsr_immediate.txt");

        jump_subroutine(instr[0]);

        REQUIRE(reg_read(Registers::PC) == g_pc_start + 20);
    }
    SECTION(" with register mode")
    {
        reg_write(Registers::PC, g_pc_start);
        reg_write(Registers::R2, g_pc_start + 15);

        std::vector<uint16_t> instr = Parser::get_instance().parse("tests/test_jsr_reg.txt");

        jump_subroutine(instr[0]);

        REQUIRE(reg_read(Registers::PC) == g_pc_start + 15);
    }
}