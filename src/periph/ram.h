#ifndef RAM_PERIPH_h
#define RAM_PERIPH_h

#include "peripheral.h"

typedef struct PeripheralRAM{
    uint8_t* memory;
    int size;
} PeripheralRAM;

void initializeRAM(Peripheral* periph);

void processRAM(Peripheral* periph, uint16_t dir, uint8_t data, uint8_t rw, uint8_t* out);

void freeRAM(Peripheral* periph);

#endif // RAM_PERIPH_h
