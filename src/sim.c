#include "sim.h"

FILE* outFile = NULL;
char outPrint[512];

void setOutputFileSimulator(FILE* file) {
    outFile = file;
}

void printMessage(char* str, ...) {
    va_list args;
    va_start(args, str);
    
    sprintf(outPrint, str, args);
    va_end(args);
    
    // Print to terminal.
    printf("%s", outPrint);
    // Save to file (if available).
    if(outFile != NULL) {
        fputs(outPrint, outFile);
    }
}

void printWarning(char* str, ...) {
    va_list args;
    va_start(args, str);
    
    int index = sprintf(outPrint, "[WARNING] ");
    sprintf(outPrint+index, str, args);
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
    sprintf(outPrint+index, str, args);
    va_end(args);
    
    // Print to terminal in red.
    printf("\e[0;31m%s\e[0m", outPrint);
    // Save to file (if available).
    if(outFile != NULL) {
        fputs(outPrint, outFile);
    }
}
