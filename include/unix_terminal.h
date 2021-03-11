#ifndef UNIX_TERMINAL_H
#define UNIX_TERMINAL_H

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/termios.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdint>
#include <iostream>

/* Unix-specific things for terminal input */

static struct termios original_tio;

uint16_t check_key();
void disable_input_buffering();
void handle_interrupt(int signal);
void restore_input_buffering();

#endif  // UNIX_TERMINAL_H