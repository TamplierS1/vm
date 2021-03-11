#include "parser.h"

Parser::Parser()
{
    // Initialize opcodes
    m_opcodes.insert({"BR", Opcodes::BR});
    m_opcodes.insert({"ADD", Opcodes::ADD});
    m_opcodes.insert({"LD", Opcodes::LD});
    m_opcodes.insert({"ST", Opcodes::ST});
    m_opcodes.insert({"JSR", Opcodes::JSR});
    m_opcodes.insert({"AND", Opcodes::AND});
    m_opcodes.insert({"LDR", Opcodes::LDR});
    m_opcodes.insert({"STR", Opcodes::STR});
    m_opcodes.insert({"RTI", Opcodes::RTI});
    m_opcodes.insert({"NOT", Opcodes::NOT});
    m_opcodes.insert({"LDI", Opcodes::LDI});
    m_opcodes.insert({"STI", Opcodes::STI});
    m_opcodes.insert({"JMP", Opcodes::JMP});
    m_opcodes.insert({"RES", Opcodes::RES});
    m_opcodes.insert({"LEA", Opcodes::LEA});
    m_opcodes.insert({"TRAP", Opcodes::TRAP});

    // Initialize registers
    m_registers.insert({"R0", Registers::R0});
    m_registers.insert({"R1", Registers::R1});
    m_registers.insert({"R2", Registers::R2});
    m_registers.insert({"R3", Registers::R3});
    m_registers.insert({"R4", Registers::R4});
    m_registers.insert({"R5", Registers::R5});
    m_registers.insert({"R6", Registers::R6});
    m_registers.insert({"R7", Registers::R7});
    m_registers.insert({"PC", Registers::PC});
    m_registers.insert({"COND", Registers::COND});
    m_registers.insert({"COUNT", Registers::COUNT});
}

std::vector<uint16_t> Parser::parse(const std::string& filename)
{
    // clear vectors of any contents left from previous parses
    m_instructions.clear();
    m_expressions.clear();

    parse_file(filename);
    parse_exprs();

    return m_instructions;
}

void Parser::parse_file(const std::string& filename)
{
    std::ifstream stream{filename};
    if (!stream.good())
    {
        std::cout << "Error: failed to open file - " << filename << '\n';
        return;
    }

    std::string line;
    while (std::getline(stream, line))
    {
        std::stringstream line_strstream{line};
        std::string word;
        Parser::Expression expr;
        // one line of expressions
        std::vector<Expression> line_of_exprs;

        while (line_strstream >> word)
        {
            try
            {
                expr = eval_word(word);
            }
            catch (const std::exception& e)
            {
                std::cout << "Error: " << e.what() << '\n';
                m_expressions.clear();
                return;
            }

            line_of_exprs.push_back(expr);
        }

        m_expressions.push_back(line_of_exprs);
    }
}

void Parser::parse_exprs()
{
    for (const auto& line_of_exprs : m_expressions)
    {
        uint16_t instruction = 0;
        // first expr in a line is always an opcode
        Opcodes opcode = std::get<Opcodes>(line_of_exprs[0]);

        switch (opcode)
        {
            case ADD:
                instruction = construct_instr<Opcodes::ADD>(line_of_exprs);
                break;
            case AND:
                instruction = construct_instr<Opcodes::AND>(line_of_exprs);
                break;
            case NOT:
                break;
            case BR:
                break;
            case JMP:
                break;
            case LD:
                break;
            case LDI:
                instruction = construct_instr<Opcodes::LDI>(line_of_exprs);
                break;
            case LDR:
                break;
            case LEA:
                break;
            case ST:
                break;
            case STI:
                break;
            case STR:
                break;
            case TRAP:
                break;
            case RES:
                break;
            case RTI:
            default:
                break;
        }

        m_instructions.push_back(instruction);
    }
}

Parser::Expression Parser::eval_word(const std::string& word) const
{
    if (m_registers.contains(word))
    {
        // if word is a register
        return m_registers.find(word)->second;
    }
    else if (m_opcodes.contains(word))
    {
        // if word is an opcode
        return m_opcodes.find(word)->second;
    }
    else if (is_number(word))
    {
        // if word is a number
        return std::stoul(word);
    }
    else
    {
        // TODO: Throw better exceptions when invalid syntax is detected
        throw "Invalid syntax.";
    }
}

bool Parser::is_number(const std::string& str) const
{
    for (char c : str)
    {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

template <Opcodes op>
uint16_t Parser::construct_instr(const std::vector<Expression>& line_of_exprs) const
{
    uint16_t instruction = 0, dr, sr1, sr2, imm5, pc_offset9;

    instruction |= op << 12;

    // TODO: add some kind of syntax checking
    // dr
    if (op == Opcodes::ADD
        || op == Opcodes::AND
        || op == Opcodes::LDI)
    {
        dr = std::get<Registers>(line_of_exprs[1]);
        instruction |= dr << 9;
    }

    // sr1
    if (op == Opcodes::ADD
        || op == Opcodes::AND)
    {
        sr1 = std::get<Registers>(line_of_exprs[2]);
        instruction |= sr1 << 6;
    }

    // immediate mode bit
    if (op == Opcodes::ADD
        || op == Opcodes::AND)
    {
        if (std::holds_alternative<uint16_t>(line_of_exprs[3]))
        {
            // set immediate mode bit
            instruction |= 1 << 5;
            imm5 = std::get<uint16_t>(line_of_exprs[3]);
            instruction |= imm5;
        }
        else
        {
            sr2 = std::get<Registers>(line_of_exprs[3]);
            instruction |= sr2;
        }
    }

    // pc_offset9
    if (op == Opcodes::LDI)
    {
       pc_offset9 = std::get<uint16_t>(line_of_exprs[2]);
       instruction |= pc_offset9;
    }

    return instruction;
}
