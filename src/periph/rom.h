#ifndef ROM_PERIPH_h
#define ROM_PERIPH_h

#include "peripheral.h"

typedef struct PeripheralROM{
    FILE *romFile;
    long romFileSize;
} PeripheralROM;

void initializeROM(Peripheral* periph, const char* romFile);

void processROM(Peripheral* periph, uint16_t dir, uint8_t data, uint8_t rw, uint8_t* out);

void freeROM(Peripheral* periph);

#endif // ROM_PERIPH_h
