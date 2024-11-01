#include <signal.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

#include "cpu.h"
#include "rom.h"
#include "ram.h"
#include "sim.h"

Peripheral rom = {
    .baseDir = 0x8000,
    .sizeDir = 0x10000,
};
Peripheral ram = {
    .baseDir = 0x0000,
    .sizeDir = 0x8000,
};
CPU cpu;

#if STORE_RUN_ON_FILE
    FILE* outputFile;    
#endif

char* romFile;
int fetchArguments(int argc, char **argv);

volatile int continueLoop = 1;
void handleSIGINT(int sig) {
    continueLoop = 0;
}

int main(int argc, char **argv) {

    if(!fetchArguments(argc, argv)) {
        exit(-1);
    }

    // Initialize peripherals.
    initializeROM(&rom, romFile);
    initializeRAM(&ram);

    // Add peripherals to the peripherals list.
    addPeripheral(&rom);
    addPeripheral(&ram);

    // Initialize CPU.
    initCPU(&cpu);
    cpu.nestingPrintIndex = NESTING_PRINT_INDEX;

#if STORE_RUN_ON_FILE
    outputFile = fopen("run.out", "w");
    cpu.outputFile = outputFile;
#endif

    // Catch signals to exit the loop securely.
    signal(SIGINT, handleSIGINT);

    // Simulate clock edge.
    while(continueLoop && !cpu.haltProgram) {
        routineCPU(&cpu);

        // #ifdef _WIN32
        //     Sleep(1000);
        // #else
        //     sleep(1);
        // #endif
    }

    freePeripherals();

#if STORE_RUN_ON_FILE
    fclose(outputFile);
#endif

    return 0;
}

int fetchArguments(int argc, char **argv) {
    if(argc != 2) {
        printf("Expected 1 argument to this function.\n");
        return 0;
    }

    romFile = argv[1];
    return 1;
}