#ifndef CPU_h
#define CPU_h

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "peripheral.h"

#define CPU_COMMENT_LENGTH 32

typedef struct CPUFlags {
    uint8_t carry       : 1;
    uint8_t zero        : 1;
    uint8_t irqDisable  : 1;
    uint8_t decimalMode : 1;
    uint8_t brkCmd      : 1;
    uint8_t reserved    : 1;
    uint8_t overflow    : 1;
    uint8_t negative    : 1;
} CPUFlags;

typedef union CPUStatus {
    CPUFlags flags;
    uint8_t val;
} CPUStatus;

typedef struct CPU {
    // Registers.
    uint8_t acc;        // Accumulator A.
    uint8_t x;          // Index X.
    uint8_t y;          // Index Y.
    uint16_t pc;        // Program Counter.
    uint8_t stack;      // LSB of Stack Pointer. MSB is always 1.
    CPUStatus status;   // Status/Flags register.

    // Statistics.
    unsigned long long clockCount;
    int nestingIndex;
    uint16_t previousPC;

    // Flags.
    // Disables printing when nesting index is greater than this value.
    int nestingPrintIndex; 
    // When set to 1, the CPU is being halted.
    uint8_t haltProgram;

    // Function comments.
    char funcComment[CPU_COMMENT_LENGTH];
} CPU;

typedef enum CPUAddressing {
    ABS_ADDRS,                      // a
    ABS_INDEXED_INDIRECT_ADDRS,     // (a,x)
    ABS_INDEXED_X_ADDRS,            // a,x
    ABS_INDEXED_Y_ADDRS,            // a,y
    ABS_INDIRECT_ADDRS,             // (a)
    ACCUMULATOR_ADDRS,              // A
    IMMEDIATE_ADDRS,                // #
    IMPLIED_ADDRS,                  // i
    PROGRAM_COUNTER_ADDRS,          // r
    STACK_ADDRS,                    // s
    ZP_ADDRS,                       // zp
    ZP_INDEXED_INDIRECT_ADDRS,      //(zp,x)
    ZP_INDEXED_X_ADDRS,             // zp,x
    ZP_INDEXED_Y_ADDRS,             // zp,y
    ZP_INDIRECT_ADDRS,              // (zp)
    ZP_INDIRECT_INDEXED_Y_ADDRS,    // (zp),y

    INVALID_ADDRS = 255,
} CPUAddressing;

typedef struct CPUInstruction {
    char            mnemonic[3];
    uint8_t         opCode;
    CPUAddressing   addressing; 
    int             byteLength;
    int             clockCycles;
    void (*callback)(CPU*, struct CPUInstruction*, uint16_t, uint8_t);
} CPUInstruction;

void initCPU(CPU* cpu);

void routineCPU(CPU* cpu);

void printInstruction(CPU* cpu, CPUInstruction* instruction, uint8_t* rawArgs, uint16_t dir, uint8_t data);

/***************************************************************************************************
 * 6502 Instructions
 **************************************************************************************************/
void ADC_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void AND_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void ASL_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void BCC_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void BCS_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void BEQ_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void BIT_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void BMI_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void BNE_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void BPL_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void BRK_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void BVC_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void BVS_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void CLC_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void CLD_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void CLI_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void CLV_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void CMP_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void CPX_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void CPY_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void DEC_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void DEX_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void DEY_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void EOR_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void INC_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void INX_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void INY_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void JMP_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void JSR_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void LDA_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void LDX_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void LDY_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void LSR_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void NOP_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void ORA_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void PHA_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void PHP_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void PLA_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void PLP_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void ROL_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void ROR_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void RTI_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void RTS_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void SBC_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void SEC_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void SED_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void SEI_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void STA_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void STX_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void STY_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void TAX_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void TAY_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void TSX_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void TXA_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void TXS_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);
void TYA_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);

#endif // CPU_h