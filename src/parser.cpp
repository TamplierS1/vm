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

    // Initialize condition flags
    m_cond_flags.insert({'n', ConditionFlags::NEG});
    m_cond_flags.insert({'z', ConditionFlags::ZRO});
    m_cond_flags.insert({'p', ConditionFlags::POS});
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
                m_instructions.clear();
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
                instruction = construct_instr<Opcodes::BR>(line_of_exprs);
                break;
            case JMP:
                instruction = construct_instr<Opcodes::JMP>(line_of_exprs);
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
        // if word is a condition flag
        /* flags are passed as one word, so we need to iterate over it
            to find out which flags were passed */
        uint16_t flags = 0;  // flags are represented as a 3-bit number, with each bit corresponding to a specific flag
        for (const char& c : word)
        {
            if (m_cond_flags.contains(c))
            {
                if (c == 'n')
                    flags |= (1 << 11);
                else if (c == 'z')
                    flags |= (1 << 10);
                else if (c == 'p')
                    flags |= (1 << 9);
            }
        }

        // the flags need to occupy the 3 left-most bits
        flags = flags >> 9;

        /* if flags are empty*/
        if (!flags)
        {
            // TODO: Throw better exceptions when invalid syntax is detected
            throw "Invalid syntax.";
        }

        return flags;
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
    uint16_t instruction = 0, dr, sr1, sr2, imm5, pc_offset9, flags, base_r;

    instruction |= op << 12;

    // TODO: add some kind of syntax checking
    // dr
    if (op == Opcodes::ADD || op == Opcodes::AND || op == Opcodes::LDI)
    {
        dr = std::get<Registers>(line_of_exprs[1]);
        instruction |= dr << 9;
    }

    // sr1
    if (op == Opcodes::ADD || op == Opcodes::AND)
    {
        sr1 = std::get<Registers>(line_of_exprs[2]);
        instruction |= sr1 << 6;
    }

    // immediate mode bit
    if (op == Opcodes::ADD || op == Opcodes::AND)
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
    if (op == Opcodes::LDI || op == Opcodes::BR)
    {
        pc_offset9 = std::get<uint16_t>(line_of_exprs[2]);
        instruction |= pc_offset9;
    }

    // condition flags
    if (op == Opcodes::BR)
    {
        flags = std::get<uint16_t>(line_of_exprs[1]);
        instruction |= flags << 9;
    }

    // base register
    if (op == Opcodes::JMP)
    {
        base_r = std::get<Registers>(line_of_exprs[1]);
        instruction |= base_r << 6;
    }
    return instruction;
}
