#ifndef SIM_h
#define SIM_h

// 0: Does not print anything.
// 1: Prints only the main.
// 2: Prints only the first calls to JSR, and not the inner JSRs from within.
// 3 and so on...: similar 
#define NESTING_PRINT_INDEX 0

// 0: Do not save the console output.
// 1: Save the console output to a file.
#define STORE_RUN_ON_FILE 0

#endif // SIM_h