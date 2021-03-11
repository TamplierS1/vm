#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "instructions.h"
#include "parser.h"

TEST_CASE(" Add instruction is executed", "[add_instruction]")
{
    SECTION(" with 2 registers mode")
    {
        reg_write(Registers::R1, 2);
        reg_write(Registers::R2, 2);

        std::vector<uint16_t> instr = Parser::get_instance().parse("tests/test_add_2_regs.txt");

        add(instr[0]);

        REQUIRE(reg_read(Registers::R0) == 4);
    }

}