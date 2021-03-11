#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <cstdint>

#include "defines.h"

// Declare functions that we need
uint16_t sign_extend(uint16_t x, unsigned int bit_count);
void update_flags(uint16_t r);
uint16_t mem_read(uint16_t address);
void mem_write(uint16_t address, uint16_t value);
uint16_t reg_read(uint16_t reg);
void reg_write(uint16_t reg, uint16_t value);

/** Add two values together and put them into *dr* (destination) register
 * There are 2 modes: immediate, 2 registers
 * Immediate: takes a value from a register and adds it to the value embedded in the instruction
 * 2 registers: takes 2 values from 2 registers and adds them together
 * */
void add(uint16_t instruction);

// Load a value to the destination register from the given memory address
void load_indirect(uint16_t instruction);

void and_bitwise(uint16_t instruction);
#endif  // INSTRUCTIONS_H
