#ifndef SIM_h
#define SIM_h

#include <stdio.h>
#include <stdarg.h>

// 0: Does not print anything.
// 1: Prints only the main.
// 2: Prints only the first calls to JSR, and not the inner JSRs from within.
// 3 and so on...: similar 
#define NESTING_PRINT_INDEX 1
#define PRINT_IN_COLOR 1

// 0: Bit 4 of STATUS register (TX Data Register Empty) is always set to 1 (like the W65C51 bug). 
// 1: Depending on the baudrate and the clock cycles of the CPU, TX Data Register Empty will be set
// to 0 until the full transmission is completed.
#define ACIA_SIMULATE_TX_WAIT 0

void setOutputFileSimulator(FILE* file);
void printMessage(char* str, ...);
void printWarning(char* str, ...);
void printError(char* str, ...);

#endif // SIM_h