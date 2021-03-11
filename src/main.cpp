#include <algorithm>
#include <array>
#include <csignal>

#include "instructions.h"
#include "parser.h"
#include "unix_terminal.h"

#define RUN_TESTS
// ! ONLY USE mem_read()/reg_read() and mem_write()/reg_write() to interact with g_memory and g_regs
// ! DO NOT TOUCH THEM DIRECTLY
std::array<uint16_t, UINT16_MAX> g_memory;
std::array<uint16_t, Registers::COUNT> regs;

// * --- UTILITY FUNCTIONS ---
// Processes terminal arguments
void process_arguments(int argc, const char* argv[]);

/** Non-16 bit values need to be sign extended to 16 bit
 * x The number that needs to be sign extended
 * bit_count The number of bits *x* has
 * */
uint16_t sign_extend(uint16_t x, unsigned int bit_count);

// Flags are updated after each instruction execution based on the value passed
void update_flags(uint16_t r);

// Read from memory at *address* and return the received value
uint16_t mem_read(uint16_t address);

// Write *value* to *address* in memory
void mem_write(uint16_t address, uint16_t value);

// Read a value from register *reg*
uint16_t reg_read(uint16_t reg);

// Write *value* to register *reg*
void reg_write(uint16_t reg, uint16_t value);

#ifndef RUN_TESTS
int main(int argc, const char* argv[])
{
    // process_arguments(argc, argv);

    signal(SIGINT, handle_interrupt);
    disable_input_buffering();

    // define pc starting position
    reg_write(Registers::PC, g_pc_start);

    std::vector<uint16_t> instr = Parser::get_instance().parse("program.txt");
    for (int i = 0; i < instr.size(); i++)
    {
        mem_write(g_pc_start + i, instr[i]);
    }

    mem_write(regs.at(Registers::PC) + 3, 4);
    mem_write(regs.at(Registers::PC) + 4, 10);

    bool is_running = true;
    while (is_running)
    {
        uint16_t instruction = mem_read(reg_read(Registers::PC));
        uint16_t opcode = instruction >> 12;

        // overflow
        if (reg_read(Registers::PC) == UINT16_MAX - 1)
        {
            std::cout << "Error: Overflow happened - too many instructions\n";
            is_running = false;
        }

        switch (opcode)
        {
            case ADD:
                add(instruction);
                break;
            case AND:
                and_bitwise(instruction);
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
                load_indirect(instruction);
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

        reg_write(Registers::PC, reg_read(Registers::PC) + 1);
    }

    restore_input_buffering();

    return 0;
}
#endif

void process_arguments(int argc, const char* argv[])
{
    if (argc < 2)
    {
        std::cout << "lc3 [image-file1] ...\n";
        exit(2);
    }

    for (int j = 1; j < argc; ++j)
    {
        //   if (!read_image(argv[j]))
        //{
        // std::cout << "Failed to load image: " << argv[j] << "\n";
        // exit(1);
        //}
    }
}

uint16_t sign_extend(uint16_t x, unsigned int bit_count)
{
    if ((x >> (bit_count - 1)) & 1)
    {
        x |= (0xFFFF << bit_count);
    }

    return x;
}

void update_flags(uint16_t r)
{
    if (r == 0)
        reg_write(Registers::COND, ConditionFlags::ZRO);
    else if (r >> 15)
        // the leftmost bit represents the number's sign
        reg_write(Registers::COND, ConditionFlags::NEG);
    else
        reg_write(Registers::COND, ConditionFlags::POS);
}

uint16_t mem_read(uint16_t address)
{
    try
    {
        return g_memory.at(address);
    }
    catch (std::out_of_range& e)
    {
        std::cout << "Error: tried to access non-existent address - " << e.what() << '\n';
        return 0;
    }
}

void mem_write(uint16_t address, uint16_t value)
{
    try
    {
        g_memory.at(address) = value;
    }
    catch (std::out_of_range& e)
    {
        std::cout << "Error: tried to access non-existent address - " << e.what() << '\n';
    }
}

uint16_t reg_read(uint16_t reg)
{
    try
    {
        return regs.at(reg);
    }
    catch (std::out_of_range& e)
    {
        std::cout << "Error: tried to access non-existent register - " << e.what() << '\n';
        return 0;
    }
}

void reg_write(uint16_t reg, uint16_t value)
{
    try
    {
        regs.at(reg) = value;
    }
    catch (std::out_of_range& e)
    {
        std::cout << "Error: tried to access non-existent register - " << e.what() << '\n';
    }
}