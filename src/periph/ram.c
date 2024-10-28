#include "ram.h"

void initializeRAM(Peripheral* periph) {
    if(periph == NULL){
        printf("Pass a valid pointer to a RAM to initialize it.\n");
        exit(-1);
    }

    PeripheralRAM* ram = (PeripheralRAM*) malloc(sizeof(PeripheralRAM));
    ram->size = periph->sizeDir - periph->baseDir;
    ram->memory = (uint8_t*) malloc(ram->size);

    // Pass the values and functions to the peripheral struct.
    periph->data = ram;
    periph->process = processRAM;
    periph->free = freeRAM;
}

void processRAM(Peripheral* periph, uint16_t dir, uint8_t data, uint8_t rw, uint8_t* out) {
    if(periph == NULL) return;

    PeripheralRAM* ram = (PeripheralRAM*) periph->data;
    int bufferDir = dir - periph->baseDir;

    if(rw) {    
        // CPU is reading from RAM.
        *out = ram->memory[bufferDir];
    }else{
        // CPU is writing to RAM.
        ram->memory[bufferDir] = data;
    }
}

void freeRAM(Peripheral* periph) {
    if(periph == NULL) return;

    PeripheralRAM* ram = (PeripheralRAM*) periph->data;
    free(ram->memory);

    free(periph->data);
}
