#include "ram.h"
#include "cpu.h"

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

void processRAM(void* pcpu, Peripheral* periph, uint16_t dir, uint8_t data, uint8_t rw, uint8_t* out) {
    if(periph == NULL) return;

    PeripheralRAM* ram = (PeripheralRAM*) periph->data;
    CPU* cpu = (CPU*) pcpu;
    int bufferDir = dir - periph->baseDir;

    if(rw) {    
        // CPU is reading from RAM.
        *out = ram->memory[bufferDir];
        sprintf(cpu->funcComment, "RAM(0x%04x): 0x%02x", dir, *out);
    }else{
        // CPU is writing to RAM.
        sprintf(cpu->funcComment, "RAM(0x%04x): 0x%02x -> 0x%02x", 
            dir, ram->memory[bufferDir], data);
        ram->memory[bufferDir] = data;
    }
}

void freeRAM(Peripheral* periph) {
    if(periph == NULL) return;

    PeripheralRAM* ram = (PeripheralRAM*) periph->data;
    free(ram->memory);

    free(periph->data);
}
