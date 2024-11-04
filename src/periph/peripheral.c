#include "peripheral.h"

Peripheral* peripheralList[PERIPHERAL_MAX_COUNT];
int peripheralCount = 0;

void addPeripheral(Peripheral* newPeripheral) {
    if(newPeripheral == NULL) {
        printError("The new peripheral is NULL\n");
        exit(-1);
    }

    if(peripheralCount == PERIPHERAL_MAX_COUNT) {
        printError("Maximum number of peripherals reached.\n");
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

void interactWithPeripheral(
    void* cpu, uint16_t direction, uint8_t data, PeripheralInteraction rw, uint8_t* out) {

    Peripheral* per;
    for(int i = 0; i < peripheralCount; i++){
        per = peripheralList[i];
        if(direction >= per->baseAddr && direction < (per->baseAddr+per->addressLen)){
            per->interact(cpu, per, direction, data, rw, out);
            return;
        }
    }

    printWarning("No peripheral found at direction 0x%x\n", direction);
}

void updatePeripherals(void* pcpu) {
    Peripheral* per;

    CPU* cpu = (CPU*) pcpu;
    cpu->irqb = 1;

    for(int i = 0; i < peripheralCount; i++){
        per = peripheralList[i];
        if(per->update != NULL) {
            per->update(cpu, per);
            
            // This will "OR" all IRQ requests.
            cpu->irqb &= per->irqb;
        }
    }
}