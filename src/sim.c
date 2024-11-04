#include "sim.h"

FILE* outFile = NULL;
char outPrint[512];

void setOutputFileSimulator(char* fileLoc) {
    if(fileLoc == NULL) return;
    outFile = fopen(fileLoc, "w");
}

void printMessage(char* str, ...) {
    va_list args;
    va_start(args, str);
    
    vsprintf(outPrint, str, args);
    va_end(args);
    
#if ENABLE_TERMINAL_PRINT
    // Print to terminal.
    printf("%s", outPrint);
#endif

    // Save to file (if available).
    if(outFile != NULL) {
        fputs(outPrint, outFile);
    }
}

void printWarning(char* str, ...) {
    va_list args;
    va_start(args, str);
    
    int index = sprintf(outPrint, "[WARNING] ");
    vsprintf(outPrint+index, str, args);
    va_end(args);
    
    // Print to terminal in yellow.
    printf("\e[0;33m%s\e[0m", outPrint);
    // Save to file (if available).
    if(outFile != NULL) {
        fputs(outPrint, outFile);
    }
}

void printError(char* str, ...) {
    va_list args;
    va_start(args, str);
    
    int index = sprintf(outPrint, "[ERROR] ");
    vsprintf(outPrint+index, str, args);
    va_end(args);
    
    // Print to terminal in red.
    printf("\e[0;31m%s\e[0m", outPrint);
    // Save to file (if available).
    if(outFile != NULL) {
        fputs(outPrint, outFile);
    }
}
