#include "rom.h"

void initializeROM(Peripheral* periph, const char* romFile) {
    if(periph == NULL){
        printError("Pass a valid pointer to a ROM to initialize it.\n");
        exit(-1);
    }

    if(romFile == NULL){
        printError("ROM file route is NULL.\n");
        exit(-1);
    }

    PeripheralROM* rom = (PeripheralROM*) malloc(sizeof(PeripheralROM));

    FILE *file = fopen(romFile, "rb");
    if(file == NULL){
        perror("Could not open ROM file");
        exit(-1);
    }

    rom->romFile = file;
    fseek(file, 0, SEEK_END);
    rom->romFileSize = ftell(file);

    if(periph->addressLen > rom->romFileSize) {
        printError("The input file size (%ld) is smaller than the size of the simulated ROM (%d). "
        "The requested content from the CPU that is out of bounds will be outputted as 0xFF.\n",
        rom->romFileSize, periph->addressLen);
    }

    // Pass the values and functions to the peripheral struct.
    periph->data = rom;
    periph->interact = interactROM;
    periph->free = freeROM;
}

void interactROM(
    void* pcpu, Peripheral* periph, uint16_t dir, uint8_t data, uint8_t rw, uint8_t* out) {
    if(periph == NULL) return;

    if(!rw){
        printWarning("Something is trying to WRITE to ROM at dir 0x%x with data 0x%x", dir, data);
        return;
    }

    PeripheralROM* rom = (PeripheralROM*) periph->data;

    int readDirection = dir - periph->baseAddr;
    if(readDirection >= rom->romFileSize){
        // Reading out of bounds, returning 0xFF.
        *out = 0xFF;
        printWarning("ROM: Reading out of bounds of binary file, at dir 0x%x", dir);
        return;
    }

    if(fseek(rom->romFile, readDirection, SEEK_SET) != 0) {
        perror("Could not seek file at ROM");
        return;
    }

    if(fread(out, 1, 1, rom->romFile) != 1) {
        perror("Could not read file at ROM");
        return;
    }
}

void freeROM(Peripheral* periph){
    if(periph == NULL) return;

    PeripheralROM* rom = (PeripheralROM*) periph->data;
    fclose(rom->romFile);

    free(periph->data);
}