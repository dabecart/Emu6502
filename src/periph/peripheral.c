#include "peripheral.h"

Peripheral* peripheralList[PERIPHERAL_MAX_COUNT];
int peripheralCount = 0;

void addPeripheral(Peripheral* newPeripheral) {
    if(newPeripheral == NULL) {
        printf("The new peripheral is NULL\n");
        exit(-1);
    }

    if(peripheralCount == PERIPHERAL_MAX_COUNT) {
        printf("Maximum number of peripherals reached.\n");
        exit(-1);
    }

    peripheralList[peripheralCount++] = newPeripheral;
}

void freePeripherals() {
    Peripheral* per;
    for(int i = 0; i < peripheralCount; i++){
        per = peripheralList[i];
        per->free(per);        
    }
}

void interactWithPeripheral(uint16_t direction, uint8_t data, PeripheralInteraction rw, uint8_t* out) {
    Peripheral* per;
    for(int i = 0; i < peripheralCount; i++){
        per = peripheralList[i];
        if(direction >= per->lowerDir && direction < per->upperDir){
            per->process(per, direction, data, rw, out);
            return;
        }
    }

    printf("No peripheral found at direction 0x%x\n", direction);
}
