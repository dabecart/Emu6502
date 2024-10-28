#ifndef PERIPHERAL_h
#define PERIPHERAL_h

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define PERIPHERAL_MAX_COUNT 5

typedef struct Peripheral {
    int baseDir;  // Included in range.
    int sizeDir;  // Not included in range.

    // Pointer to an struct holding the data for the specific peripheral. Cast it to the 
    // correspondent type to use it.
    void* data;

    // The process function as arguments:
    // - Peripheral* periph. Pointer to the current peripheral.
    // - uint16_t direction. The value on the direction bus.
    // - uint8_t data. The value on the data bus.
    // - uint8_t R/#W. On 1 is reading, on 0 is writing.
    // - uint8_t* output. If R/#W = 1, something should be outputted here. The pointer will always
    // be given by the called to this function, that is, this pointer cannot be NULL if reading.
    void (*process)(struct Peripheral*, uint16_t, uint8_t, uint8_t, uint8_t*);

    // Cleanup function.
    void (*free)(struct Peripheral*);
} Peripheral;

typedef enum PeripheralInteraction {
    WRITE_PERIPH = 0,
    READ_PERIPH = 1,
} PeripheralInteraction;

/****************************************** FUNCTION ***********************************************
 \brief Adds a peripheral to the list of peripherals. This will be used to easily process the 
 peripheral once the CPU requires it.
 \param newPeripheral. The peripheral to be added.
 \return None.
***************************************************************************************************/
void addPeripheral(Peripheral* newPeripheral);

/****************************************** FUNCTION ***********************************************
 \brief Clears the peripherals list. 
 \return None.
***************************************************************************************************/
void freePeripherals();

/****************************************** FUNCTION ***********************************************
 \brief Reads or writes data to a peripheral.
 \param direction. Value of the direction buffer.
 \param data. Value of the data buffer.
 \param rw. If 0, the CPU is reading from the peripheral. If other than 1, it's writing to it.
 \param out. If something is being expected to be received over the data bus, this will be written 
 to this pointer.
 \return None.
***************************************************************************************************/
void interactWithPeripheral(uint16_t direction, uint8_t data, 
                            PeripheralInteraction rw, uint8_t* out);

#endif // PERIPHERAL_h