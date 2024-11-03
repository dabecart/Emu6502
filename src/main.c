#include <signal.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

#include "sim.h"
#include "cpu.h"
#include "rom.h"
#include "ram.h"
#include "acia.h"

Peripheral rom = {
    .baseAddr = 0xC000,
    .addressLen = 16*1024,
};
Peripheral ram = {
    .baseAddr = 0x0000,
    .addressLen = 16*1024,
};
Peripheral acia1 = {
    .baseAddr = 0xA000,
    .addressLen = 4,
};
CPU cpu;

char* romFile = NULL;
char* outputFile = NULL;
char* serialRoute = NULL;
void printHelp();
void fetchArguments(int argc, char **argv);

volatile int continueLoop = 1;
void handleSIGINT(int sig) {
    continueLoop = 0;
}

int main(int argc, char **argv) {

    fetchArguments(argc, argv);

    // Initialize peripherals.
    initializeROM(&rom, romFile);
    initializeRAM(&ram);
    initializeACIA(&acia1);
    setSerialACIA(&acia1, serialRoute);

    // Add peripherals to the peripherals list.
    addPeripheral(&rom);
    addPeripheral(&ram);
    addPeripheral(&acia1);

    // Initialize CPU.
    initCPU(&cpu);
    setNestingPrintIndexCPU(&cpu, NESTING_PRINT_INDEX);
    FILE* fout = setOutputFileCPU(&cpu, outputFile);
    setOutputFileSimulator(fout);

    // Catch signals to exit the loop securely.
    signal(SIGINT, handleSIGINT);

    // Prints a new line that will be cut by the printInstruction function inside routineCPU.
    printf("\n");

    // Simulate clock edge.
    while(continueLoop && !cpu.haltProgram) {
        routineCPU(&cpu);

        updatePeripherals(&cpu);

        // #ifdef _WIN32
        //     Sleep(1000);
        // #else
        //     sleep(1);
        // #endif
    }

    freePeripherals();
    freeCPU(&cpu);

    return 0;
}

void printHelp() {
    printf("emu6502, by @dabecart. 2024.\n");
    printf("Usage:   emu6502 -h <-o file> <-s serial> romFile\n");
    printf("Arguments:\n");
    printf("      romFile       Binary file with the content of ROM.\n");

    printf("Optional arguments:\n");
    printf("      -h            Prints this help menu.\n");
    printf("      -o file       Stores the runtime output of the simulation to file.\n");
    printf("      -s serial     Specifies the \"serial port\" to communicate via console with the simulator.\n");
}

void fetchArguments(int argc, char **argv) {
    char* arg;
    int argIndex;
    for(argIndex = 1; argIndex < argc; argIndex++) {
        arg = argv[argIndex];
        if(arg[0] == '-') {
            if(arg[1] == 'h') {
                printHelp();
                exit(0);
            }else if(arg[1] == 'o') {
                if((++argIndex) < argc) {
                    outputFile = argv[argIndex];
                }else {
                    printError("Missing output (-o) file argument. Use -h to list usages.\n");
                    exit(-1);
                }
            }else if(arg[1] == 's') {
                if((++argIndex) < argc) {
                    serialRoute = argv[argIndex];
                }else {
                    printError("Missing serial (-s) port. Use -h to list usages.\n");
                    exit(-1);
                }
            }
        }else{
            break;
        }
    }

    // Last argument will be the romFile.
    romFile = argv[argIndex];
}