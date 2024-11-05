#ifndef SIM_h
#define SIM_h

#include <stdio.h>
#include <stdarg.h>

// 0: Does not print anything.
// 1: Prints only the main.
// 2: Prints only the first calls to JSR, and not the inner JSRs from within.
// 3 and so on...: similar 
#define NESTING_PRINT_INDEX     25
// Set to 1 to print to the terminal. Makes the program slower.
#define ENABLE_TERMINAL_PRINT   0
// Set to 1 to print to the terminal with colors.
#define PRINT_IN_COLOR          1

#define SIMULATE_W65C02S        1

// 0: Bit 4 of STATUS register (TX Data Register Empty) is always set to 1 (like the W65C51 bug).
//    Makes it faster to get a response from the 6502.
// 1: Depending on the baudrate and the clock cycles of the CPU, TX Data Register Empty will be set
//    to 0 until the full transmission is completed.
//    Takes more time to get a response from the 6502. The CPU cycle count is more realistic.
#define ACIA_SIMULATE_TX_WAIT 0

// You may paste content in the serial terminal and it will be sent (almost) inmediately, therefore
// the simulator always does a wait inbetween incomming bytes. This wait is calculated from the CPU
// cycles and its running frequency.
// 0: Defaults to the wait of a 115200 bauds communication port (the fastest baudrate of the ACIA). 
//    Makes the simulator more responsive when inputting data.
// 1: Wait time calculated from the real baudrate of the communication port.
//    Inputting data is slower, but makes the simulator CPU cycle count more realistic.
#define ACIA_SIMULATE_RX_WAIT 0

void setOutputFileSimulator(char* file);
void printMessage(char* str, ...);
void printWarning(char* str, ...);
void printError(char* str, ...);

#endif // SIM_h