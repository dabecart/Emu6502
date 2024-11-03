; **************************************************************************************************
; acia_demo.s
; **************************************************************************************************

ACIA_DATA   = $A000
ACIA_STATUS = $A001
ACIA_CMD    = $A002
ACIA_CTRL   = $A003

  .org $E000
test_str:
  .text "Hello"

  .org $C000
reset:
  sei				  
  ldx #$FF  ; INITIALIZING THE STACK POINTER
  txs
  cld       ; Clear decimal mode.

  sta ACIA_STATUS ; SW reset of the ACIA
  lda #%00001110  ; 1 stop bit, length 8 bits, 9600 bauds
  sta ACIA_CTRL
  lda #%00000010  ; No parity, no echo, no IRQ
  sta ACIA_CMD

  ldx 0
print:
  lda test_str,x
  sta ACIA_DATA
  pha
wait_for_clear:
  lda ACIA_STATUS
  and #$10
  beq wait_for_clear  ; Wait until the ACIA has send the byte.
  inx
  pla
  bne print

  brk   ; End of program


  .org $fffc
  .word reset
  .word $0000