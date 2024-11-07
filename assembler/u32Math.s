; **************************************************************************************************
; u32Math.s
; **************************************************************************************************

    .org $E000
operands:
    .byte $5a       ; OP1 LSB
    .byte $aa
    .byte $5a
    .byte $5a       ; OP1 MSB

    .byte $5a       ; OP2 LSB
    .byte $00
    .byte $00
    .byte $00       ; OP2 MSB

OP1         = 0
OP2         = 4
RESULT      = 8     ; 8 bytes
AUX         = 16    ; 4 bytes accumulator aux

    .org $C000

reset:
  SEI				  
  LDX #$FF  ; INITIALIZING THE STACK POINTER
  TXS
  CLD       ; Clear decimal mode.

  JSR set_vals
  JSR div_u32

  LDA RESULT
  LDA RESULT+1
  LDA RESULT+2
  LDA RESULT+3

  brk

set_vals:
  ldx #7
set_vals_loop:
  lda operands,x
  sta OP1,x
  dex
  bpl set_vals_loop
  rts

clear_results_region:
  ldx #11
clear_results_region_loop:
  stz RESULT,x
  dex
  bpl clear_results_region_loop
  rts

  .org $fffc
  .word reset
  .word $0000