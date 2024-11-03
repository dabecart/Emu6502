#include "acia.h"

void initializeACIA(Peripheral* periph) {
    if(periph == NULL){
        printError("Pass a valid pointer to an ACIA to initialize it.\n");
        exit(-1);
    }

    PeripheralACIA* acia = (PeripheralACIA*) malloc(sizeof(PeripheralACIA));
    memset(acia, 0, sizeof(PeripheralACIA));    // Set initially all to zero.
    acia->serial = -1;

    // Initial status after hardware reset of the ACIA.
    // All zero except...
    acia->command.irqEnable = 1;
    acia->status.txEmpty = 1;

    // Pass the values and functions to the peripheral struct.
    periph->data = acia;
    periph->interact = interactACIA;
    periph->update = updateACIA;
    periph->free = freeACIA;
}

void interactACIA(
    void* pcpu, Peripheral* periph, uint16_t dir, uint8_t data, uint8_t rw, uint8_t* out) {

    if(pcpu == NULL || periph == NULL) return;

    CPU* cpu = (CPU*) pcpu;
    PeripheralACIA* acia = (PeripheralACIA*) periph->data;
    switch (dir - periph->baseAddr) {
    case ACIA_REG_DATA: {
        if(rw) {    
            // Reading: returns the RX buffer.
            *out = acia->rxData;
            acia->status.rxFull = 0;    // The data has been read, RX buffer will be emptied out.
            acia->status.overrunError = 0;
            sprintf(cpu->funcComment, "ACIA: read 0x%02x", *out);
        }else {     
            // Writing: sends the TX buffer.
            if(!acia->status.txEmpty) {
#if ACIA_SIMULATE_TX_WAIT
                unsigned long cpuDeltaTX = cpu->clockCount - acia->cpuClockOnTX;
                unsigned long serialDeltaTX = cpuDeltaTX * acia->baudrate / CPU_CLK_SPEED;
                if(serialDeltaTX < acia->txLength) {
                    // Not enough time has passed for the byte to be transmitted.
                    int missingCycles = acia->txLength - serialDeltaTX + 1;
                    printWarning("ACIA data that still has not been sent (0x%x), overwriten by "
                                    "0x%x Add %d cycle(s) more between instructions.\n", 
                                    acia->txData, data, missingCycles);
                }
#else 
                    printWarning("ACIA data that still has not been sent (0x%x), overwriten by "
                                    "0x%x.\n", 
                                    acia->txData, data);
#endif
            }
            acia->txData = data;
            sprintf(cpu->funcComment, "ACIA: write 0x%02x", acia->txData);
            // Send data on the updateACIA function.
            acia->txPending = 1;
        }
        break;
    }

    case ACIA_REG_STATUS: {
        if(rw) {
            // Reading: Outputs to the CPU the status register.
            memcpy(out, &acia->status, 1);
            sprintf(cpu->funcComment, "ACIA: read status");
        }else {
            // Writing: software reset of the ACIA (not all fields get reset to 0).
            acia->status.dataCarrierDetect = 0;
            acia->command.echoEnabled = 0;
            acia->command.txControls = ACIA_TXC_IRQoff_RTSBhigh;
            acia->command.irqEnable = 1;    // Disabled.
            acia->command.dtrReady = 0;
            acia->rxData = 0;
            acia->txData = 0;

            sprintf(cpu->funcComment, "ACIA: software reset");
        }
        break;
    }

    case ACIA_REG_COMMAND: {
        if(rw) {
            // Reading: Outputs to the CPU the command register.
            memcpy(out, &acia->command, 1);
        }else {
            // Writing.
            int commentIndex = sprintf(cpu->funcComment, "ACIA:");

            CommandRegACIA inReg;
            memcpy(&inReg, &data, sizeof(CommandRegACIA));
            if(inReg.dtrReady != acia->command.dtrReady) {
                printWarning("DTR# is not implemented.\n");
            }

            if(inReg.irqEnable != acia->command.irqEnable) {
                commentIndex += sprintf(cpu->funcComment+commentIndex, " IRQ: ");
                if(inReg.irqEnable) {
                    commentIndex += sprintf(cpu->funcComment+commentIndex, "OFF");
                }else {
                    commentIndex += sprintf(cpu->funcComment+commentIndex, "ON");
                }
            }

            if(inReg.txControls != acia->command.txControls) {
                commentIndex += sprintf(cpu->funcComment+commentIndex, " TX: ");
                switch (inReg.txControls)
                {
                case ACIA_TXC_IRQoff_RTSBhigh: {
                    commentIndex += sprintf(cpu->funcComment+commentIndex, "IRQ off, RTS# high.");
                    break;
                }

                case ACIA_TXC_IRQon_RTSBlow: {
                    commentIndex += sprintf(cpu->funcComment+commentIndex, "IRQ on, RTS# low.");
                    break;
                }

                case ACIA_TXC_IRQoff_RTSBlow_BRK: {
                    printWarning("ACIA Transmitter control 11: TX IRQ disabled, RTS low and BRK is "
                    "not implemented. Fallen back to 10: TX IRQ disabled, RTS low (no BRK). \n");
                    inReg.txControls = ACIA_TXC_IRQoff_RTSBlow;
                }
                case ACIA_TXC_IRQoff_RTSBlow: {
                    commentIndex += sprintf(cpu->funcComment+commentIndex, "IRQ off, RTS# high.");
                    break;
                }

                default:
                    printError("Undefined TX Control value in ACIA.\n");
                    cpu->haltProgram = 1;
                    break;
                }

                if(inReg.echoEnabled != acia->command.echoEnabled) {
                    commentIndex += sprintf(cpu->funcComment+commentIndex, " ECHO:");
                    if(inReg.echoEnabled) {
                        commentIndex += sprintf(cpu->funcComment+commentIndex, "ON");
                    }else {
                        commentIndex += sprintf(cpu->funcComment+commentIndex, "OFF");
                    }
                }

                if(inReg.parity != acia->command.parity) {
                    commentIndex += sprintf(cpu->funcComment+commentIndex, " PARITY:");
                    switch (inReg.parity) {
                    case ACIA_PARITY_OFF: {
                        commentIndex += sprintf(cpu->funcComment+commentIndex, "OFF");
                        break;
                    }

                    case ACIA_PARITY_ODD: {
                        commentIndex += sprintf(cpu->funcComment+commentIndex, "ODD");
                        break;
                    }

                    case ACIA_PARITY_EVEN: {
                        commentIndex += sprintf(cpu->funcComment+commentIndex, "EVEN");
                        break;
                    }

                    case ACIA_PARITY_MARK: {
                        commentIndex += sprintf(cpu->funcComment+commentIndex, "MARK");
                        break;
                    }

                    case ACIA_PARITY_SPACE: {
                        commentIndex += sprintf(cpu->funcComment+commentIndex, "SPACE");
                        break;
                    }

                    default:
                        printError("Undefined Parity Control value in ACIA.\n");
                        cpu->haltProgram = 1;
                    }
                }
            }

            // Store command in ACIA.
            acia->command = inReg;
            // Apply any changes.
            setSerialConfigurationACIA_(acia);
        }
        break;
    }

    case ACIA_REG_CONTROL: {
        if(rw) {
            // Reading: Outputs to the CPU the control register.
            memcpy(out, &acia->control, 1);
        }else {
            // Writing.
            int commentIndex = sprintf(cpu->funcComment, "ACIA:");

            ControlRegACIA inReg;
            memcpy(&inReg, &data, sizeof(ControlRegACIA));
            if(inReg.baudrate != acia->control.baudrate) {
                int baudrate = aciaBaudrateToInt_(inReg.baudrate);
                int termiosBaudrate = intBaudrateToTermios_(baudrate);
                if(termiosBaudrate < 0) {
                    // Fall back to the previous serial port baudrate.
                    baudrate = aciaBaudrateToInt_(acia->control.baudrate);
                    termiosBaudrate = intBaudrateToTermios_(baudrate);
                }
                commentIndex += sprintf(cpu->funcComment+commentIndex, " BAUDS:%d", baudrate);
            }

            if(inReg.clockSource != acia->control.clockSource) {
                printWarning("Receiver Clock Source not implemented in ACIA.\n");
            }

            if(inReg.wordLength != acia->control.wordLength) {
                commentIndex += sprintf(cpu->funcComment+commentIndex, " LEN:");
                switch(inReg.wordLength) {
                    case ACIA_WL_8: commentIndex += sprintf(cpu->funcComment+commentIndex, "8b");
                    case ACIA_WL_7: commentIndex += sprintf(cpu->funcComment+commentIndex, "7b");
                    case ACIA_WL_6: commentIndex += sprintf(cpu->funcComment+commentIndex, "6b");
                    case ACIA_WL_5: commentIndex += sprintf(cpu->funcComment+commentIndex, "5b");
                    default: {
                        printError("Undefined Word Length in ACIA.\n");
                        cpu->haltProgram = 1;
                    }
                }
            }

            if(inReg.stopBits != acia->control.stopBits) {
                commentIndex += sprintf(cpu->funcComment+commentIndex, " STOP:");
                switch(inReg.stopBits) {
                    case ACIA_SB_1: commentIndex += sprintf(cpu->funcComment+commentIndex, "1b");
                    case ACIA_SB_2: commentIndex += sprintf(cpu->funcComment+commentIndex, "2b");
                    default: {
                        printError("Undefined Stop Bits in ACIA.\n");
                        cpu->haltProgram = 1;
                    }
                }
            }

            // Store control in ACIA.
            acia->control = inReg;
            // Apply any changes.
            setSerialConfigurationACIA_(acia);
        }
        break;
    }

    default:
        printWarning("Invalid register direction %d in ACIA.\n", dir-periph->baseAddr);
        break;
    }
}

void updateACIA(void* pcpu, Peripheral* periph) {
    if(periph == NULL) return;

    CPU* cpu = (CPU*) pcpu;
    PeripheralACIA* acia = (PeripheralACIA*) periph->data;

    if(acia->txPending) {
#if ACIA_SIMULATE_TX_WAIT
        // CPU requested to send. Check if there's been enough time for the previous byte to be 
        // transmitted.
        unsigned long cpuDeltaTX = cpu->clockCount - acia->cpuClockOnTX;
        unsigned long serialDeltaTX = cpuDeltaTX * acia->baudrate / CPU_CLK_SPEED;
        if(serialDeltaTX < acia->txLength) {
            // Not enough time has passed for the byte to be transmitted.
            int missingCycles = acia->txLength - serialDeltaTX + 1;
            printWarning("ACIA: Not enough time between bytes! Add %d cycle(s) more.\n", 
                missingCycles);
        }
        acia->status.txEmpty = 0;
#endif

        writeToSerialACIA_(acia, &acia->txData, 1);
        acia->txPending = 0;
        acia->cpuClockOnTX = cpu->clockCount;
    }

#if ACIA_SIMULATE_TX_WAIT
    // Check if enough cycles have passed and the TX empty flag can be restored.
    if(!acia->status.txEmpty) {
        unsigned long cpuDeltaTX = cpu->clockCount - acia->cpuClockOnTX;
        unsigned long serialDeltaTX = cpuDeltaTX * acia->baudrate / CPU_CLK_SPEED;
        if(serialDeltaTX >= acia->txLength) {
            // Enough time has passed for the byte to be transmitted.
            acia->status.txEmpty = 1;
        }
    }
#else 
    acia->status.txEmpty = 1;
#endif

    // char tempRead;
    // int rxByteCount = readFromSerialACIA_(acia, 1, &tempRead);
    // if(rxByteCount == 1) {
    //     acia->rxData = tempRead;
    //     // Data has been overwritten.
    //     if(acia->status.rxFull) acia->status.overrunError = 1;
        
    //     acia->status.rxFull = 1;
    // }
}

void freeACIA(Peripheral* periph) {
    if(periph == NULL) return;

    PeripheralACIA* acia = (PeripheralACIA*) periph->data;
    if(acia->serial >= 0) {
        close(acia->serial);
    }

    free(periph->data);
}

void setSerialACIA(Peripheral* periph, char* serialRoute) {
    if(periph == NULL) return;

    PeripheralACIA* acia = (PeripheralACIA*) periph->data;
    acia->serial = open(serialRoute, O_RDWR | O_NOCTTY | O_SYNC | O_NONBLOCK);
    if (acia->serial < 0) {
        fprintf(stderr, "%s > ", serialRoute);
        perror("Error opening ACIA serial port");
        exit(-1);
    }
    
    // Set initial configuration.
    setSerialConfigurationACIA_(acia);

    // Configure individual and static port settings.
    struct termios tty;
    if (tcgetattr(acia->serial, &tty) != 0) {
        perror("Error getting ACIA serial port attributes");
        close(acia->serial);
        exit(-1);
    }
    
    tty.c_iflag &= ~IGNBRK;                         // Disable break processing.
    tty.c_lflag = 0;                                // No signaling chars, no echo.
    tty.c_oflag = 0;                                // No remapping, no delays.
    tty.c_cc[VMIN]  = 1;                            // Read at least 1 character.
    tty.c_cc[VTIME] = 5;                            // 0.5 seconds read timeout.

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);         // Shut off xon/xoff ctrl.

    tty.c_cflag |= (CLOCAL | CREAD);                // Enable reading.
    tty.c_cflag &= ~CRTSCTS;                        // Disable CRS/CTS.

    if (tcsetattr(acia->serial, TCSANOW, &tty) != 0) {
        perror("Error setting ACIA serial port attributes");
        close(acia->serial);
        exit(-1);
    }

    const char *msg = "######## ACIA connected ########\n";
    writeToSerialACIA_(acia, msg, strlen(msg));
}

int aciaBaudrateToInt_(int aciaBauds) {
    switch (aciaBauds) {
        case ACIA_BR_115200:    return 115200;
        case ACIA_BR_50:        return 50;
        case ACIA_BR_75:        return 75;
        case ACIA_BR_134_58:    return 134;    
        case ACIA_BR_150:       return 150;
        case ACIA_BR_300:       return 300;
        case ACIA_BR_600:       return 600;
        case ACIA_BR_1200:      return 1200;
        case ACIA_BR_1800:      return 1800;
        case ACIA_BR_2400:      return 2400;
        case ACIA_BR_4800:      return 4800;
        case ACIA_BR_9600:      return 9600;
        case ACIA_BR_19200:     return 19200;
        
        case ACIA_BR_109_92:    return 109;
        case ACIA_BR_7200:      return 7200;
        case ACIA_BR_3600:      return 3600;
        default:
            printWarning("ACIA Baudrate not valid in aciaBaudrateToInt_.\n");
    }
    return -1;
}

int intBaudrateToTermios_(int bauds) {
    switch (bauds) {
        case 115200:    return B115200;
        case 50:        return B50;
        case 75:        return B75;
        case 134:       return B134;    
        case 150:       return B150;
        case 300:       return B300;
        case 600:       return B600;
        case 1200:      return B1200;
        case 1800:      return B1800;
        case 2400:      return B2400;
        case 4800:      return B4800;
        case 9600:      return B9600;
        case 19200:     return B19200;
        
        case 109:
        case 7200:
        case 3600:
        default:
            printWarning("int baudrate %d not valid in intBaudrateToTermios_.\n", bauds);
    }
    return -1;
}

int setSerialConfigurationACIA_(PeripheralACIA *acia) {
    int stopBits, wordLength;
    int parity = acia->command.parity != ACIA_PARITY_OFF;       // Parity bit count.
    int baudrate = aciaBaudrateToInt_(acia->control.baudrate);

    struct termios options;

    // Get the current configuration of the serial interface.
    if (tcgetattr(acia->serial, &options) != 0) {
        perror("Error getting current serial settings");
        return -1;
    }

    int termiosBaud = intBaudrateToTermios_(baudrate);
    cfsetospeed(&options, termiosBaud);  // Set output speed.
    cfsetispeed(&options, termiosBaud);  // Set input speed.

    switch (acia->control.stopBits) {
    case ACIA_SB_1: options.c_cflag &= ~CSTOPB;     stopBits = 1;       break;
    case ACIA_SB_2: options.c_cflag |= CSTOPB;      stopBits = 2;       break;
    default:
        printError("Invalid stop bits selection.\n");
        return -1;
    }

    switch (acia->command.parity) {
        case ACIA_PARITY_OFF: // None
            options.c_cflag &= ~PARENB;
            break;
        case ACIA_PARITY_ODD: // Odd
            options.c_cflag |= PARENB;
            options.c_cflag |= PARODD;
            break;
        case ACIA_PARITY_EVEN: // Even
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;
            break;
        case ACIA_PARITY_MARK: // Mark
            options.c_cflag |= PARENB;
            options.c_cflag |= PARODD;
            options.c_cflag |= CSTOPB;
            break;
        case ACIA_PARITY_SPACE: // Space
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~PARODD;
            options.c_cflag |= CSTOPB;
            break;
        default:
            printError("Invalid parity selection.\n");
            return -1;
    }

    options.c_cflag = (options.c_cflag & ~CSIZE);
    switch (acia->control.wordLength) {
        case ACIA_WL_8: options.c_cflag |= CS8;     wordLength = 8;     break;
        case ACIA_WL_7: options.c_cflag |= CS7;     wordLength = 7;     break;
        case ACIA_WL_6: options.c_cflag |= CS6;     wordLength = 6;     break;
        case ACIA_WL_5: options.c_cflag |= CS5;     wordLength = 5;     break;
    default:
        printError("Invalid word length selection.\n");
        return -1;
    }

    // Apply the modified settings immediately.
    if (tcsetattr(acia->serial, TCSANOW, &options) != 0) {
        perror("Error setting new serial settings");
        return -1;
    }

    // If reached here, the settings were correct. Modify the acia struct.
    acia->txLength = 1 + wordLength + parity + stopBits;
    acia->baudrate = baudrate;

    return 0;
}

int writeToSerialACIA_(PeripheralACIA *acia, const char* msg, int len) {
    int writtenBytes = write(acia->serial, msg, len);
    if (writtenBytes < 0) {
        perror("Error writing to ACIA serial port");
    }
    return writtenBytes;
}

int readFromSerialACIA_(PeripheralACIA *acia, int len, char* msg) {
    int readBytes = read(acia->serial, msg, len);
    if (readBytes < 0) {
        perror("Error reading from ACIA serial port");
    }
    return readBytes;
}
