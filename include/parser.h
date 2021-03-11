#ifndef PARSER_H
#define PARSER_H

#include <bitset>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <variant>
#include <vector>

#include "defines.h"

class Parser
{
public:
    Parser(const Parser&) = delete;
    void operator=(const Parser&) = delete;

    static Parser& get_instance()
    {
        static Parser instance;
        return instance;
    }

    std::vector<uint16_t> parse(const std::string& filename);

private:
    using Expression = std::variant<Registers, Opcodes, uint16_t>;

    Parser();

    void parse_file(const std::string& filename);
    void parse_exprs();
    [[nodiscard]] Expression eval_word(const std::string& word) const;

    [[nodiscard]] bool is_number(const std::string& str) const;

    /* Naming conventions:
     * dr = destination register
     * sr1 = source register 1
     * sr2 = source register 2
     * imm5 = 5-bit immediate value
     * pc_offfset9 = 9-bit offset to program counter*/
    // Constructs binary instructions from expressions
    // *op* is the opcode of the instruction
    template <Opcodes op>
    [[nodiscard]] uint16_t construct_instr(const std::vector<Expression>& line_of_exprs) const;

    std::vector<uint16_t> m_instructions;
    // vector of lines of expressions
    std::vector<std::vector<Expression>> m_expressions;

    std::map<std::string, Opcodes> m_opcodes;
    std::map<std::string, Registers> m_registers;
};

#endif