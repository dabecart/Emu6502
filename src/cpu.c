#include "cpu.h"

const CPUInstruction instructions[] = {
    {"BRK", 0x00, IMPLIED_ADDRS,                 1, 7, BRK_ins_},  // BRK: Force Break
    {"ORA", 0x01, ZP_INDEXED_INDIRECT_ADDRS,     2, 6, ORA_ins_},  // ORA: OR Memory with Accumulator
    {"IND", 0x02, INVALID_ADDRS},
    {"IND", 0x03, INVALID_ADDRS},
    {"IND", 0x04, INVALID_ADDRS},
    {"ORA", 0x05, ZP_ADDRS,                      2, 3, ORA_ins_},  // ORA: OR Memory with Accumulator
    {"ASL", 0x06, ZP_ADDRS,                      2, 5, ASL_ins_},  // ASL: Shift Left One Bit (Memory or Accumulator)
    {"IND", 0x07, INVALID_ADDRS},
    {"PHP", 0x08, IMPLIED_ADDRS,                 1, 3, PHP_ins_},  // PHP: Push Processor Status on Stack
    {"ORA", 0x09, IMMEDIATE_ADDRS,               2, 2, ORA_ins_},  // ORA: OR Memory with Accumulator
    {"ASL", 0x0a, ACCUMULATOR_ADDRS,             1, 2, ASL_ins_},  // ASL: Shift Left One Bit (Memory or Accumulator)
    {"IND", 0x0b, INVALID_ADDRS},
    {"IND", 0x0c, INVALID_ADDRS},
    {"ORA", 0x0d, ABS_ADDRS,                     3, 4, ORA_ins_},  // ORA: OR Memory with Accumulator
    {"ASL", 0x0e, ABS_ADDRS,                     3, 6, ASL_ins_},  // ASL: Shift Left One Bit (Memory or Accumulator)
    {"IND", 0x0f, INVALID_ADDRS},
    {"BPL", 0x10, PROGRAM_COUNTER_ADDRS,         2, 2, BPL_ins_},  // BPL: Branch on Result Plus
    {"ORA", 0x11, ZP_INDIRECT_INDEXED_Y_ADDRS,   2, 5, ORA_ins_},  // ORA: OR Memory with Accumulator
    {"IND", 0x12, INVALID_ADDRS},
    {"IND", 0x13, INVALID_ADDRS},
    {"IND", 0x14, INVALID_ADDRS},
    {"ORA", 0x15, ZP_INDEXED_X_ADDRS,            2, 4, ORA_ins_},  // ORA: OR Memory with Accumulator
    {"ASL", 0x16, ZP_INDEXED_X_ADDRS,            2, 6, ASL_ins_},  // ASL: Shift Left One Bit (Memory or Accumulator)
    {"IND", 0x17, INVALID_ADDRS},
    {"CLC", 0x18, IMPLIED_ADDRS,                 1, 2, CLC_ins_},  // CLC: Clear Carry Flag
    {"ORA", 0x19, ABS_INDEXED_Y_ADDRS,           3, 4, ORA_ins_},  // ORA: OR Memory with Accumulator
    {"IND", 0x1a, INVALID_ADDRS},
    {"IND", 0x1b, INVALID_ADDRS},
    {"IND", 0x1c, INVALID_ADDRS},
    {"ORA", 0x1d, ABS_INDEXED_X_ADDRS,           3, 4, ORA_ins_},  // ORA: OR Memory with Accumulator
    {"ASL", 0x1e, ABS_INDEXED_X_ADDRS,           3, 7, ASL_ins_},  // ASL: Shift Left One Bit (Memory or Accumulator)
    {"IND", 0x1f, INVALID_ADDRS},
    {"JSR", 0x20, ABS_ADDRS,                     3, 6, JSR_ins_},  // JSR: Jump to New Location Saving Return Address
    {"AND", 0x21, ZP_INDEXED_INDIRECT_ADDRS,     2, 6, AND_ins_},  // AND: AND Memory with Accumulator
    {"IND", 0x22, INVALID_ADDRS},
    {"IND", 0x23, INVALID_ADDRS},
    {"BIT", 0x24, ZP_ADDRS,                      2, 3, BIT_ins_},  // BIT: Test Bits in Memory with Accumulator
    {"AND", 0x25, ZP_ADDRS,                      2, 3, AND_ins_},  // AND: AND Memory with Accumulator
    {"ROL", 0x26, ZP_ADDRS,                      2, 5, ROL_ins_},  // ROL: Rotate One Bit Left (Memory or Accumulator)
    {"IND", 0x27, INVALID_ADDRS},
    {"PLP", 0x28, IMPLIED_ADDRS,                 1, 4, PLP_ins_},  // PLP: Pull Processor Status from Stack
    {"AND", 0x29, IMMEDIATE_ADDRS,               2, 2, AND_ins_},  // AND: AND Memory with Accumulator
    {"ROL", 0x2a, ACCUMULATOR_ADDRS,             1, 2, ROL_ins_},  // ROL: Rotate One Bit Left (Memory or Accumulator)
    {"IND", 0x2b, INVALID_ADDRS},
    {"BIT", 0x2c, ABS_ADDRS,                     3, 4, BIT_ins_},  // BIT: Test Bits in Memory with Accumulator
    {"AND", 0x2d, ABS_ADDRS,                     3, 4, AND_ins_},  // AND: AND Memory with Accumulator
    {"ROL", 0x2e, ABS_ADDRS,                     3, 6, ROL_ins_},  // ROL: Rotate One Bit Left (Memory or Accumulator)
    {"IND", 0x2f, INVALID_ADDRS},
    {"BMI", 0x30, PROGRAM_COUNTER_ADDRS,         2, 2, BMI_ins_},  // BMI: Branch on Result Minus
    {"AND", 0x31, ZP_INDIRECT_INDEXED_Y_ADDRS,   2, 5, AND_ins_},  // AND: AND Memory with Accumulator
    {"IND", 0x32, INVALID_ADDRS},
    {"IND", 0x33, INVALID_ADDRS},
    {"IND", 0x34, INVALID_ADDRS},
    {"AND", 0x35, ZP_INDEXED_X_ADDRS,            2, 4, AND_ins_},  // AND: AND Memory with Accumulator
    {"ROL", 0x36, ZP_INDEXED_X_ADDRS,            2, 6, ROL_ins_},  // ROL: Rotate One Bit Left (Memory or Accumulator)
    {"IND", 0x37, INVALID_ADDRS},
    {"SEC", 0x38, IMPLIED_ADDRS,                 1, 2, SEC_ins_},  // SEC: Set Carry Flag
    {"AND", 0x39, ABS_INDEXED_Y_ADDRS,           3, 4, AND_ins_},  // AND: AND Memory with Accumulator
    {"IND", 0x3a, INVALID_ADDRS},
    {"IND", 0x3b, INVALID_ADDRS},
    {"IND", 0x3c, INVALID_ADDRS},
    {"AND", 0x3d, ABS_INDEXED_X_ADDRS,           3, 4, AND_ins_},  // AND: AND Memory with Accumulator
    {"ROL", 0x3e, ABS_INDEXED_X_ADDRS,           3, 7, ROL_ins_},  // ROL: Rotate One Bit Left (Memory or Accumulator)
    {"IND", 0x3f, INVALID_ADDRS},
    {"RTI", 0x40, IMPLIED_ADDRS,                 1, 6, RTI_ins_},  // RTI: Return from Interrupt
    {"EOR", 0x41, ZP_INDEXED_INDIRECT_ADDRS,     2, 6, EOR_ins_},  // EOR: Exclusive-OR Memory with Accumulator
    {"IND", 0x42, INVALID_ADDRS},
    {"IND", 0x43, INVALID_ADDRS},
    {"IND", 0x44, INVALID_ADDRS},
    {"EOR", 0x45, ZP_ADDRS,                      2, 3, EOR_ins_},  // EOR: Exclusive-OR Memory with Accumulator
    {"LSR", 0x46, ZP_ADDRS,                      2, 5, LSR_ins_},  // LSR: Shift One Bit Right (Memory or Accumulator)
    {"IND", 0x47, INVALID_ADDRS},
    {"PHA", 0x48, IMPLIED_ADDRS,                 1, 3, PHA_ins_},  // PHA: Push Accumulator on Stack
    {"EOR", 0x49, IMMEDIATE_ADDRS,               2, 2, EOR_ins_},  // EOR: Exclusive-OR Memory with Accumulator
    {"LSR", 0x4a, ACCUMULATOR_ADDRS,             1, 2, LSR_ins_},  // LSR: Shift One Bit Right (Memory or Accumulator)
    {"IND", 0x4b, INVALID_ADDRS},
    {"JMP", 0x4c, ABS_ADDRS,                     3, 3, JMP_ins_},  // JMP: Jump to New Location
    {"EOR", 0x4d, ABS_ADDRS,                     3, 4, EOR_ins_},  // EOR: Exclusive-OR Memory with Accumulator
    {"LSR", 0x4e, ABS_ADDRS,                     3, 6, LSR_ins_},  // LSR: Shift One Bit Right (Memory or Accumulator)
    {"IND", 0x4f, INVALID_ADDRS},
    {"BVC", 0x50, PROGRAM_COUNTER_ADDRS,         2, 2, BVC_ins_},  // BVC: Branch on Overflow Clear
    {"EOR", 0x51, ZP_INDIRECT_INDEXED_Y_ADDRS,   2, 5, EOR_ins_},  // EOR: Exclusive-OR Memory with Accumulator
    {"IND", 0x52, INVALID_ADDRS},
    {"IND", 0x53, INVALID_ADDRS},
    {"IND", 0x54, INVALID_ADDRS},
    {"EOR", 0x55, ZP_INDEXED_X_ADDRS,            2, 4, EOR_ins_},  // EOR: Exclusive-OR Memory with Accumulator
    {"LSR", 0x56, ZP_INDEXED_X_ADDRS,            2, 6, LSR_ins_},  // LSR: Shift One Bit Right (Memory or Accumulator)
    {"IND", 0x57, INVALID_ADDRS},
    {"CLI", 0x58, IMPLIED_ADDRS,                 1, 2, CLI_ins_},  // CLI: Clear Interrupt Disable Bit
    {"EOR", 0x59, ABS_INDEXED_Y_ADDRS,           3, 4, EOR_ins_},  // EOR: Exclusive-OR Memory with Accumulator
    {"IND", 0x5a, INVALID_ADDRS},
    {"IND", 0x5b, INVALID_ADDRS},
    {"IND", 0x5c, INVALID_ADDRS},
    {"EOR", 0x5d, ABS_INDEXED_X_ADDRS,           3, 4, EOR_ins_},  // EOR: Exclusive-OR Memory with Accumulator
    {"LSR", 0x5e, ABS_INDEXED_X_ADDRS,           3, 7, LSR_ins_},  // LSR: Shift One Bit Right (Memory or Accumulator)
    {"IND", 0x5f, INVALID_ADDRS},
    {"RTS", 0x60, IMPLIED_ADDRS,                 1, 6, RTS_ins_},  // RTS: Return from Subroutine
    {"ADC", 0x61, ZP_INDEXED_INDIRECT_ADDRS,     2, 6, ADC_ins_},  // ADC: Add Memory to Accumulator with Carry
    {"IND", 0x62, INVALID_ADDRS},
    {"IND", 0x63, INVALID_ADDRS},
    {"IND", 0x64, INVALID_ADDRS},
    {"ADC", 0x65, ZP_ADDRS,                      2, 3, ADC_ins_},  // ADC: Add Memory to Accumulator with Carry
    {"ROR", 0x66, ZP_ADDRS,                      2, 5, ROR_ins_},  // ROR: Rotate One Bit Right (Memory or Accumulator)
    {"IND", 0x67, INVALID_ADDRS},
    {"PLA", 0x68, IMPLIED_ADDRS,                 1, 4, PLA_ins_},  // PLA: Pull Accumulator from Stack
    {"ADC", 0x69, IMMEDIATE_ADDRS,               2, 2, ADC_ins_},  // ADC: Add Memory to Accumulator with Carry
    {"ROR", 0x6a, ACCUMULATOR_ADDRS,             1, 2, ROR_ins_},  // ROR: Rotate One Bit Right (Memory or Accumulator)
    {"IND", 0x6b, INVALID_ADDRS},
    {"JMP", 0x6c, ABS_INDIRECT_ADDRS,            3, 5, JMP_ins_},  // JMP: Jump to New Location
    {"ADC", 0x6d, ABS_ADDRS,                     3, 4, ADC_ins_},  // ADC: Add Memory to Accumulator with Carry
    {"ROR", 0x6e, ABS_ADDRS,                     3, 6, ROR_ins_},  // ROR: Rotate One Bit Right (Memory or Accumulator)
    {"IND", 0x6f, INVALID_ADDRS},
    {"BVS", 0x70, PROGRAM_COUNTER_ADDRS,         2, 2, BVS_ins_},  // BVS: Branch on Overflow Set
    {"ADC", 0x71, ZP_INDIRECT_INDEXED_Y_ADDRS,   2, 5, ADC_ins_},  // ADC: Add Memory to Accumulator with Carry
    {"IND", 0x72, INVALID_ADDRS},
    {"IND", 0x73, INVALID_ADDRS},
    {"IND", 0x74, INVALID_ADDRS},
    {"ADC", 0x75, ZP_INDEXED_X_ADDRS,            2, 4, ADC_ins_},  // ADC: Add Memory to Accumulator with Carry
    {"ROR", 0x76, ZP_INDEXED_X_ADDRS,            2, 6, ROR_ins_},  // ROR: Rotate One Bit Right (Memory or Accumulator)
    {"IND", 0x77, INVALID_ADDRS},
    {"SEI", 0x78, IMPLIED_ADDRS,                 1, 2, SEI_ins_},  // SEI: Set Interrupt Disable Status
    {"ADC", 0x79, ABS_INDEXED_Y_ADDRS,           3, 4, ADC_ins_},  // ADC: Add Memory to Accumulator with Carry
    {"IND", 0x7a, INVALID_ADDRS},
    {"IND", 0x7b, INVALID_ADDRS},
    {"IND", 0x7c, INVALID_ADDRS},
    {"ADC", 0x7d, ABS_INDEXED_X_ADDRS,           3, 4, ADC_ins_},  // ADC: Add Memory to Accumulator with Carry
    {"ROR", 0x7e, ABS_INDEXED_X_ADDRS,           3, 7, ROR_ins_},  // ROR: Rotate One Bit Right (Memory or Accumulator)
    {"IND", 0x7f, INVALID_ADDRS},
    {"IND", 0x80, INVALID_ADDRS},
    {"STA", 0x81, ZP_INDEXED_INDIRECT_ADDRS,     2, 6, STA_ins_},  // STA: Store Accumulator in Memory
    {"IND", 0x82, INVALID_ADDRS},
    {"IND", 0x83, INVALID_ADDRS},
    {"STY", 0x84, ZP_ADDRS,                      2, 3, STY_ins_},  // STY: Sore Index Y in Memory
    {"STA", 0x85, ZP_ADDRS,                      2, 3, STA_ins_},  // STA: Store Accumulator in Memory
    {"STX", 0x86, ZP_ADDRS,                      2, 3, STX_ins_},  // STX: Store Index X in Memory
    {"IND", 0x87, INVALID_ADDRS},
    {"DEY", 0x88, IMPLIED_ADDRS,                 1, 2, DEY_ins_},  // DEY: Decrement Index Y by One
    {"IND", 0x89, INVALID_ADDRS},
    {"TXA", 0x8a, IMPLIED_ADDRS,                 1, 2, TXA_ins_},  // TXA: Transfer Index X to Accumulator
    {"IND", 0x8b, INVALID_ADDRS},
    {"STY", 0x8c, ABS_ADDRS,                     3, 4, STY_ins_},  // STY: Sore Index Y in Memory
    {"STA", 0x8d, ABS_ADDRS,                     3, 4, STA_ins_},  // STA: Store Accumulator in Memory
    {"STX", 0x8e, ABS_ADDRS,                     3, 4, STX_ins_},  // STX: Store Index X in Memory
    {"IND", 0x8f, INVALID_ADDRS},
    {"BCC", 0x90, PROGRAM_COUNTER_ADDRS,         2, 2, BCC_ins_},  // BCC: Branch on Carry Clear
    {"STA", 0x91, ZP_INDIRECT_INDEXED_Y_ADDRS,   2, 6, STA_ins_},  // STA: Store Accumulator in Memory
    {"IND", 0x92, INVALID_ADDRS},
    {"IND", 0x93, INVALID_ADDRS},
    {"STY", 0x94, ZP_INDEXED_X_ADDRS,            2, 4, STY_ins_},  // STY: Sore Index Y in Memory
    {"STA", 0x95, ZP_INDEXED_X_ADDRS,            2, 4, STA_ins_},  // STA: Store Accumulator in Memory
    {"STX", 0x96, ZP_INDEXED_Y_ADDRS,            2, 4, STX_ins_},  // STX: Store Index X in Memory
    {"IND", 0x97, INVALID_ADDRS},
    {"TYA", 0x98, IMPLIED_ADDRS,                 1, 2, TYA_ins_},  // TYA: Transfer Index Y to Accumulator
    {"STA", 0x99, ABS_INDEXED_Y_ADDRS,           3, 5, STA_ins_},  // STA: Store Accumulator in Memory
    {"TXS", 0x9a, IMPLIED_ADDRS,                 1, 2, TXS_ins_},  // TXS: Transfer Index X to Stack Register
    {"IND", 0x9b, INVALID_ADDRS},
    {"IND", 0x9c, INVALID_ADDRS},
    {"STA", 0x9d, ABS_INDEXED_X_ADDRS,           3, 5, STA_ins_},  // STA: Store Accumulator in Memory
    {"IND", 0x9e, INVALID_ADDRS},
    {"IND", 0x9f, INVALID_ADDRS},
    {"LDY", 0xa0, IMMEDIATE_ADDRS,               2, 2, LDY_ins_},  // LDY: Load Index Y with Memory
    {"LDA", 0xa1, ZP_INDEXED_INDIRECT_ADDRS,     2, 6, LDA_ins_},  // LDA: Load Accumulator with Memory
    {"LDX", 0xa2, IMMEDIATE_ADDRS,               2, 2, LDX_ins_},  // LDX: Load Index X with Memory
    {"IND", 0xa3, INVALID_ADDRS},
    {"LDY", 0xa4, ZP_ADDRS,                      2, 3, LDY_ins_},  // LDY: Load Index Y with Memory
    {"LDA", 0xa5, ZP_ADDRS,                      2, 3, LDA_ins_},  // LDA: Load Accumulator with Memory
    {"LDX", 0xa6, ZP_ADDRS,                      2, 3, LDX_ins_},  // LDX: Load Index X with Memory
    {"IND", 0xa7, INVALID_ADDRS},
    {"TAY", 0xa8, IMPLIED_ADDRS,                 1, 2, TAY_ins_},  // TAY: Transfer Accumulator to Index Y
    {"LDA", 0xa9, IMMEDIATE_ADDRS,               2, 2, LDA_ins_},  // LDA: Load Accumulator with Memory
    {"TAX", 0xaa, IMPLIED_ADDRS,                 1, 2, TAX_ins_},  // TAX: Transfer Accumulator to Index X
    {"IND", 0xab, INVALID_ADDRS},
    {"LDY", 0xac, ABS_ADDRS,                     3, 4, LDY_ins_},  // LDY: Load Index Y with Memory
    {"LDA", 0xad, ABS_ADDRS,                     3, 4, LDA_ins_},  // LDA: Load Accumulator with Memory
    {"LDX", 0xae, ABS_ADDRS,                     3, 4, LDX_ins_},  // LDX: Load Index X with Memory
    {"IND", 0xaf, INVALID_ADDRS},
    {"BCS", 0xb0, PROGRAM_COUNTER_ADDRS,         2, 2, BCS_ins_},  // BCS: Branch on Carry Set
    {"LDA", 0xb1, ZP_INDIRECT_INDEXED_Y_ADDRS,   2, 5, LDA_ins_},  // LDA: Load Accumulator with Memory
    {"IND", 0xb2, INVALID_ADDRS},
    {"IND", 0xb3, INVALID_ADDRS},
    {"LDY", 0xb4, ZP_INDEXED_X_ADDRS,            2, 4, LDY_ins_},  // LDY: Load Index Y with Memory
    {"LDA", 0xb5, ZP_INDEXED_X_ADDRS,            2, 4, LDA_ins_},  // LDA: Load Accumulator with Memory
    {"LDX", 0xb6, ZP_INDEXED_Y_ADDRS,            2, 4, LDX_ins_},  // LDX: Load Index X with Memory
    {"IND", 0xb7, INVALID_ADDRS},
    {"CLV", 0xb8, IMPLIED_ADDRS,                 1, 2, CLV_ins_},  // CLV: Clear Overflow Flag
    {"LDA", 0xb9, ABS_INDEXED_Y_ADDRS,           3, 4, LDA_ins_},  // LDA: Load Accumulator with Memory
    {"TSX", 0xba, IMPLIED_ADDRS,                 1, 2, TSX_ins_},  // TSX: Transfer Stack Pointer to Index X
    {"IND", 0xbb, INVALID_ADDRS},
    {"LDY", 0xbc, ABS_INDEXED_X_ADDRS,           3, 4, LDY_ins_},  // LDY: Load Index Y with Memory
    {"LDA", 0xbd, ABS_INDEXED_X_ADDRS,           3, 4, LDA_ins_},  // LDA: Load Accumulator with Memory
    {"LDX", 0xbe, ABS_INDEXED_Y_ADDRS,           3, 4, LDX_ins_},  // LDX: Load Index X with Memory
    {"IND", 0xbf, INVALID_ADDRS},
    {"CPY", 0xc0, IMMEDIATE_ADDRS,               2, 2, CPY_ins_},  // CPY: Compare Memory and Index Y
    {"CMP", 0xc1, ZP_INDEXED_INDIRECT_ADDRS,     2, 6, CMP_ins_},  // CMP: Compare Memory with Accumulator
    {"IND", 0xc2, INVALID_ADDRS},
    {"IND", 0xc3, INVALID_ADDRS},
    {"CPY", 0xc4, ZP_ADDRS,                      2, 3, CPY_ins_},  // CPY: Compare Memory and Index Y
    {"CMP", 0xc5, ZP_ADDRS,                      2, 3, CMP_ins_},  // CMP: Compare Memory with Accumulator
    {"DEC", 0xc6, ZP_ADDRS,                      2, 5, DEC_ins_},  // DEC: Decrement Memory by One
    {"IND", 0xc7, INVALID_ADDRS},
    {"INY", 0xc8, IMPLIED_ADDRS,                 1, 2, INY_ins_},  // INY: Increment Index Y by One
    {"CMP", 0xc9, IMMEDIATE_ADDRS,               2, 2, CMP_ins_},  // CMP: Compare Memory with Accumulator
    {"DEX", 0xca, IMPLIED_ADDRS,                 1, 2, DEX_ins_},  // DEX: Decrement Index X by One
    {"IND", 0xcb, INVALID_ADDRS},
    {"CPY", 0xcc, ABS_ADDRS,                     3, 4, CPY_ins_},  // CPY: Compare Memory and Index Y
    {"CMP", 0xcd, ABS_ADDRS,                     3, 4, CMP_ins_},  // CMP: Compare Memory with Accumulator
    {"DEC", 0xce, ABS_ADDRS,                     3, 6, DEC_ins_},  // DEC: Decrement Memory by One
    {"IND", 0xcf, INVALID_ADDRS},
    {"BNE", 0xd0, PROGRAM_COUNTER_ADDRS,         2, 2, BNE_ins_},  // BNE: Branch on Result not Zero
    {"CMP", 0xd1, ZP_INDIRECT_INDEXED_Y_ADDRS,   2, 5, CMP_ins_},  // CMP: Compare Memory with Accumulator
    {"IND", 0xd2, INVALID_ADDRS},
    {"IND", 0xd3, INVALID_ADDRS},
    {"IND", 0xd4, INVALID_ADDRS},
    {"CMP", 0xd5, ZP_INDEXED_X_ADDRS,            2, 4, CMP_ins_},  // CMP: Compare Memory with Accumulator
    {"DEC", 0xd6, ZP_INDEXED_X_ADDRS,            2, 6, DEC_ins_},  // DEC: Decrement Memory by One
    {"IND", 0xd7, INVALID_ADDRS},
    {"CLD", 0xd8, IMPLIED_ADDRS,                 1, 2, CLD_ins_},  // CLD: Clear Decimal Mode
    {"CMP", 0xd9, ABS_INDEXED_Y_ADDRS,           3, 4, CMP_ins_},  // CMP: Compare Memory with Accumulator
    {"IND", 0xda, INVALID_ADDRS},
    {"IND", 0xdb, INVALID_ADDRS},
    {"IND", 0xdc, INVALID_ADDRS},
    {"CMP", 0xdd, ABS_INDEXED_X_ADDRS,           3, 4, CMP_ins_},  // CMP: Compare Memory with Accumulator
    {"DEC", 0xde, ABS_INDEXED_X_ADDRS,           3, 7, DEC_ins_},  // DEC: Decrement Memory by One
    {"IND", 0xdf, INVALID_ADDRS},
    {"CPX", 0xe0, IMMEDIATE_ADDRS,               2, 2, CPX_ins_},  // CPX: Compare Memory and Index X
    {"SBC", 0xe1, ZP_INDEXED_INDIRECT_ADDRS,     2, 6, SBC_ins_},  // SBC: Subtract Memory from Accumulator with Borrow
    {"IND", 0xe2, INVALID_ADDRS},
    {"IND", 0xe3, INVALID_ADDRS},
    {"CPX", 0xe4, ZP_ADDRS,                      2, 3, CPX_ins_},  // CPX: Compare Memory and Index X
    {"SBC", 0xe5, ZP_ADDRS,                      2, 3, SBC_ins_},  // SBC: Subtract Memory from Accumulator with Borrow
    {"INC", 0xe6, ZP_ADDRS,                      2, 5, INC_ins_},  // INC: Increment Memory by One
    {"IND", 0xe7, INVALID_ADDRS},
    {"INX", 0xe8, IMPLIED_ADDRS,                 1, 2, INX_ins_},  // INX: Increment Index X by One
    {"SBC", 0xe9, IMMEDIATE_ADDRS,               2, 2, SBC_ins_},  // SBC: Subtract Memory from Accumulator with Borrow
    {"NOP", 0xea, IMPLIED_ADDRS,                 1, 2, NOP_ins_},  // NOP: No Operation
    {"IND", 0xeb, INVALID_ADDRS},
    {"CPX", 0xec, ABS_ADDRS,                     3, 4, CPX_ins_},  // CPX: Compare Memory and Index X
    {"SBC", 0xed, ABS_ADDRS,                     3, 4, SBC_ins_},  // SBC: Subtract Memory from Accumulator with Borrow
    {"INC", 0xee, ABS_ADDRS,                     3, 6, INC_ins_},  // INC: Increment Memory by One
    {"IND", 0xef, INVALID_ADDRS},
    {"BEQ", 0xf0, PROGRAM_COUNTER_ADDRS,         2, 2, BEQ_ins_},  // BEQ: Branch on Result Zero
    {"SBC", 0xf1, ZP_INDIRECT_INDEXED_Y_ADDRS,   2, 5, SBC_ins_},  // SBC: Subtract Memory from Accumulator with Borrow
    {"IND", 0xf2, INVALID_ADDRS},
    {"IND", 0xf3, INVALID_ADDRS},
    {"IND", 0xf4, INVALID_ADDRS},
    {"SBC", 0xf5, ZP_INDEXED_X_ADDRS,            2, 4, SBC_ins_},  // SBC: Subtract Memory from Accumulator with Borrow
    {"INC", 0xf6, ZP_INDEXED_X_ADDRS,            2, 6, INC_ins_},  // INC: Increment Memory by One
    {"IND", 0xf7, INVALID_ADDRS},
    {"SED", 0xf8, IMPLIED_ADDRS,                 1, 2, SED_ins_},  // SED: Set Decimal Flag
    {"SBC", 0xf9, ABS_INDEXED_Y_ADDRS,           3, 4, SBC_ins_},  // SBC: Subtract Memory from Accumulator with Borrow
    {"IND", 0xfa, INVALID_ADDRS},
    {"IND", 0xfb, INVALID_ADDRS},
    {"IND", 0xfc, INVALID_ADDRS},
    {"SBC", 0xfd, ABS_INDEXED_X_ADDRS,           3, 4, SBC_ins_},  // SBC: Subtract Memory from Accumulator with Borrow
    {"INC", 0xfe, ABS_INDEXED_X_ADDRS,           3, 7, INC_ins_},  // INC: Increment Memory by One
    {"IND", 0xff, INVALID_ADDRS},
};

void initCPU(CPU* cpu) {
    if(cpu == NULL){
        printf("Pass a valid pointer to a CPU to initialize it.\n");
        exit(-1);
    }

    // Initialize registers.
    cpu->acc = 0;
    cpu->x = 0;
    cpu->y = 0;
    cpu->stack = 0;
    cpu->status.val = 0;

    // Get the RESB vector (the initial memory position of the program).
    interactWithPeripheral(0xFFFC, 0, READ_PERIPH, (uint8_t*) (&cpu->pc));
    interactWithPeripheral(0xFFFD, 0, READ_PERIPH, ((uint8_t*) (&cpu->pc)) + 1);

    printf("CPU starting from 0x%04x\n", cpu->pc);

    cpu->clockCount = 0;
}

void routineCPU(CPU* cpu) {
    // Get the OP code.
    uint8_t opCode;
    interactWithPeripheral(cpu->pc, 0, READ_PERIPH, &opCode);
    
    CPUInstruction instruction = instructions[opCode];
    if(instruction.addressing == INVALID_ADDRS){
        printf("Invalid instruction found at PC: 0x%04x", cpu->pc);
        exit(-1);
    }

    // Get the instruction arguments. 
    uint16_t rawArgs = 0;
    // This will only work on little endian devices!
    uint8_t* rawArgsBuffer = (uint8_t*) (&rawArgs);
    for(int i = 0; i < instruction.byteLength - 1; i++) {
        interactWithPeripheral(cpu->pc + 1 + i, 0, READ_PERIPH, rawArgsBuffer+i);
    }

    // Calculate the operation argument/direction based on the addressing type and the instruction
    // arguments.
    uint16_t opDirection = rawArgs;
    uint8_t opData = 0;

    switch(instruction.addressing) {
        case IMMEDIATE_ADDRS:
            opData = rawArgsBuffer[0];
        break;

        case IMPLIED_ADDRS:
        case STACK_ADDRS:
            // Stack operations only modify the stack. No data required.
            break;

        case ACCUMULATOR_ADDRS:
            opData = cpu->acc;
        break;

        case PROGRAM_COUNTER_ADDRS:
            // Only used on branch instructions. If the branch is taken, jump to opData.
            opData = opDirection + cpu->pc;
        break;

        case ABS_INDIRECT_ADDRS:    // Only used on JMP instruction.
        case ABS_ADDRS:
        case ZP_ADDRS:
            interactWithPeripheral(opDirection, 0, READ_PERIPH, &opData);
        break;

        case ABS_INDEXED_X_ADDRS:
        case ZP_INDEXED_X_ADDRS:
            opDirection += cpu->x;
            interactWithPeripheral(opDirection, 0, READ_PERIPH, &opData);
        break;

        case ABS_INDEXED_Y_ADDRS:
        case ZP_INDEXED_Y_ADDRS:
            opDirection += cpu->y;
            interactWithPeripheral(opDirection, 0, READ_PERIPH, &opData);
        break;

        case ZP_INDIRECT_ADDRS:
            interactWithPeripheral(opDirection, 0, READ_PERIPH, (uint8_t*) &opDirection);
            interactWithPeripheral(opDirection, 0, READ_PERIPH, &opData);
        break;

        case ABS_INDEXED_INDIRECT_ADDRS:
            // Only used on JMP instruction (new mode created on the 65C02).
            // Same as below with the difference that the opDirection here is a 16 bit value, and on
            // the ZP is only eight bits.

        case ZP_INDEXED_INDIRECT_ADDRS:
            interactWithPeripheral(opDirection + cpu->x, 0, READ_PERIPH, (uint8_t*) &opDirection);
            interactWithPeripheral(opDirection, 0, READ_PERIPH, &opData);
        break;

        case ZP_INDIRECT_INDEXED_Y_ADDRS:
            interactWithPeripheral(opDirection, 0, READ_PERIPH, (uint8_t*) &opDirection);
            opDirection += cpu->y;
            interactWithPeripheral(opDirection, 0, READ_PERIPH, &opData);
        break;

        default:
            printf("Undefined addressing at PC 0x%04x", cpu->pc);
            exit(-1);
    }

    // Once the instruction and its arguments are parsed, call the operation.
    instruction.callback(cpu, &instruction, opDirection, opData);

    printInstruction(cpu, &instruction, rawArgsBuffer, opDirection, opData);

    // Increment the PC.
    cpu->pc += instruction.byteLength;
}

void printInstruction(CPU* cpu, CPUInstruction* instruction, uint8_t* rawArgs, uint16_t dir, uint8_t data) {
    printf("0x%04x: %02x  ", cpu->pc, instruction->opCode);
    for(int i = 1; i < 3; i++) {
        if(i <= instruction->byteLength){
            printf("%02x  ", *(rawArgs+i-1));
        }else{
            printf("    ");
        }
    }

    char mnemonic[4] = {instruction->mnemonic[0], instruction->mnemonic[1], instruction->mnemonic[2], 0};
    printf("    -   %s ", mnemonic);
    
    switch(instruction->addressing) {
        case ABS_ADDRS:
            printf("#$%04x", dir);
        break;

        case ABS_INDEXED_INDIRECT_ADDRS:

        break;

        case ABS_INDEXED_X_ADDRS:

        break;

        case ABS_INDEXED_Y_ADDRS:

        break;

        case ABS_INDIRECT_ADDRS:

        break;

        case ACCUMULATOR_ADDRS:

        break;

        case IMMEDIATE_ADDRS:
            printf("#$%04x", dir);
        break;

        case IMPLIED_ADDRS:

        break;

        case PROGRAM_COUNTER_ADDRS:

        break;

        case STACK_ADDRS:

        break;

        case ZP_ADDRS:

        break;

        case ZP_INDEXED_INDIRECT_ADDRS:

        break;

        case ZP_INDEXED_X_ADDRS:

        break;

        case ZP_INDEXED_Y_ADDRS:

        break;

        case ZP_INDIRECT_ADDRS:

        break;

        case ZP_INDIRECT_INDEXED_Y_ADDRS:

        break;

    }

    printf("\n");
}


void ADC_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void AND_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void ASL_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void BCC_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void BCS_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void BEQ_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void BIT_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void BMI_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void BNE_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void BPL_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void BRK_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void BVC_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void BVS_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void CLC_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void CLD_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void CLI_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void CLV_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void CMP_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void CPX_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void CPY_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void DEC_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void DEX_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void DEY_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void EOR_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void INC_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void INX_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void INY_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void JMP_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void JSR_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void LDA_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void LDX_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void LDY_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void LSR_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void NOP_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void ORA_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void PHA_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void PHP_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void PLA_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void PLP_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void ROL_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void ROR_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void RTI_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void RTS_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void SBC_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void SEC_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void SED_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void SEI_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void STA_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void STX_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void STY_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void TAX_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void TAY_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void TSX_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void TXA_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void TXS_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}

void TYA_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data) {

}
