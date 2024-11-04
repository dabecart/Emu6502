#ifndef ACIA_PERIPH_h
#define ACIA_PERIPH_h

#include "peripheral.h"
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

typedef enum ClockSourceACIA {
    ACIA_CS_EXTERNAL = 0,
    ACIA_CS_BAUD     = 1,
} ClockSourceACIA;

typedef enum BaudRateACIA {
    ACIA_BR_115200       = 0,
    ACIA_BR_50,
    ACIA_BR_75,
    ACIA_BR_109_92,
    ACIA_BR_134_58,
    ACIA_BR_150,
    ACIA_BR_300,
    ACIA_BR_600,
    ACIA_BR_1200,
    ACIA_BR_1800,
    ACIA_BR_2400,
    ACIA_BR_3600,
    ACIA_BR_4800,
    ACIA_BR_7200,
    ACIA_BR_9600,
    ACIA_BR_19200,
} BaudRateACIA;

typedef enum WordLengthACIA {
    ACIA_WL_8        = 0,
    ACIA_WL_7,
    ACIA_WL_6,
    ACIA_WL_5,
} WordLengthACIA;

typedef enum StopBitsACIA {
    ACIA_SB_1       = 0,
    ACIA_SB_2       = 1,
} StopBitsACIA;

typedef struct ControlRegACIA {
    BaudRateACIA    baudrate    : 4;
    ClockSourceACIA clockSource : 1;
    WordLengthACIA  wordLength  : 2;
    StopBitsACIA    stopBits    : 1;
} ControlRegACIA;

typedef enum TXControlsACIA {
    ACIA_TXC_IRQoff_RTSBhigh    = 0,
    ACIA_TXC_IRQon_RTSBlow,
    ACIA_TXC_IRQoff_RTSBlow,
    ACIA_TXC_IRQoff_RTSBlow_BRK     // Not implemented, same as ACIA_TXC_IRQoff_RTSBlow.
} TXControlsACIA;

typedef enum ParityControlsACIA {
    ACIA_PARITY_OFF             = 0,
    ACIA_PARITY_ODD             = 1,
    ACIA_PARITY_EVEN            = 3,
    ACIA_PARITY_MARK            = 5,
    ACIA_PARITY_SPACE           = 7,
} ParityControlsACIA;

typedef struct CommandRegACIA {
    uint8_t             dtrReady    : 1;    // Not used.
    uint8_t             irqEnable   : 1;    // 0: IRQ interrupt enabled. 1: IRQ interrupt disabled.
    TXControlsACIA      txControls  : 2;
    uint8_t             echoEnabled : 1;
    ParityControlsACIA  parity      : 3;
} CommandRegACIA;

typedef struct StatusRegACIA {
    uint8_t parityError         : 1;
    uint8_t framingError        : 1;
    uint8_t overrunError        : 1;
    uint8_t rxFull              : 1;
    uint8_t txEmpty             : 1;
    uint8_t dataCarrierDetect   : 1;        // Not implemented nor used.
    uint8_t dataSetReady        : 1;        // Not implemented nor used.
    uint8_t irq                 : 1;
} StatusRegACIA;

typedef enum ACIARegisters {
    ACIA_REG_DATA = 0,
    ACIA_REG_STATUS,
    ACIA_REG_COMMAND,
    ACIA_REG_CONTROL,
} ACIARegisters;

typedef struct PeripheralACIA {
    uint8_t txData;
    uint8_t rxData;
    StatusRegACIA status;
    ControlRegACIA control;
    CommandRegACIA command;
    
    int serial;         // Serial file descriptor.
    int txPending;
    unsigned long long cpuClockOnTX;
    unsigned long long cpuClockOnRX;

    int commsBitLength; // Number of bits sent per communications byte.
    int baudrate;       // Numerical baudrate.
} PeripheralACIA;

void initializeACIA(Peripheral* periph);

void interactACIA(void* pcpu, Peripheral* periph, uint16_t dir, uint8_t data, uint8_t rw, uint8_t* out);

void updateACIA(void* pcpu, Peripheral* periph);

void freeACIA(Peripheral* periph);

void setSerialACIA(Peripheral* periph, char* serialRoute);

int aciaBaudrateToInt_(int aciaBauds);

int intBaudrateToTermios_(int bauds);

int setSerialConfigurationACIA_(PeripheralACIA *acia);

int writeToSerialACIA_(PeripheralACIA *acia, const char* msg, int len);

int readFromSerialACIA_(PeripheralACIA *acia, int len, char* msg);

#endif // ACIA_PERIPH_h
