#include "rom.h"

void initializeROM(Peripheral* periph, const char* romFile) {
    if(periph == NULL){
        printf("Pass a valid pointer to a ROM to initialize it.\n");
        exit(-1);
    }

    if(romFile == NULL){
        printf("ROM file route is NULL.\n");
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

    int directionRange = periph->sizeDir - periph->baseDir;
    if(directionRange > rom->romFileSize) {
        printf("The input file size (%ld) is smaller than the size of the simulated ROM (%d). The "
        "requested content from the CPU that is out of bounds will be outputted as 0xFF.\n",
        rom->romFileSize, directionRange);
    }

    // Pass the values and functions to the peripheral struct.
    periph->data = rom;
    periph->process = processROM;
    periph->free = freeROM;
}

void processROM(
    void* pcpu, Peripheral* periph, uint16_t dir, uint8_t data, uint8_t rw, uint8_t* out) {
    if(periph == NULL) return;

    if(!rw){
        printf("Something is trying to WRITE to ROM at dir 0x%x with data 0x%x", dir, data);
        return;
    }

    PeripheralROM* rom = (PeripheralROM*) periph->data;

    int readDirection = dir - periph->baseDir;
    if(readDirection >= rom->romFileSize){
        // Reading out of bounds, returning 0xFF.
        *out = 0xFF;
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