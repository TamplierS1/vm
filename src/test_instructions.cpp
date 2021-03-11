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

    mem_write(g_pc_start + 5, g_pc_start + 6);
    mem_write(g_pc_start + 6, 10);

    std::vector<uint16_t> instr = Parser::get_instance().parse("tests/test_ldi.txt");

    load_indirect(instr[0]);

    REQUIRE(reg_read(Registers::R0) == 10);
}

TEST_CASE(" AND instruction is executed", "[and_instruction")
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