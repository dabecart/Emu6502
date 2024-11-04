; **************************************************************************************************
; acia_demo.s
; Prints a hello message and awaits user input. Once the user presses enter, the program prints the
; buffer's content.
; **************************************************************************************************

ACIA_DATA   = $A000
ACIA_STATUS = $A001
ACIA_CMD    = $A002
ACIA_CTRL   = $A003

BUFF        = $0200   ; Input buffer of 256 bytes.
BUFF_HEAD   = $20     ; Write position
BUFF_TAIL   = $21     ; Read position
BUFF_PRINT  = $22     ; Position of input buffer that's been printed.

  .org $E000
test_str:
  .text "emu6502 start!"
  .byte $0D           ; Carriage return
  .byte $0A           ; New line
  .text "< "

  .org $C000
reset:
  cld       ; Clear decimal mode.
  ldx #$FF  ; INITIALIZING THE STACK POINTER
  txs
  cli       ; Allow interruptions

  ; Initialize both buffer pointers to the same location.
  lda BUFF_HEAD
  sta BUFF_TAIL

  sta ACIA_STATUS ; SW reset of the ACIA
  lda #%00001110  ; 1 stop bit, length 8 bits, 9600 bauds
  sta ACIA_CTRL
  lda #%00000000  ; No parity, no echo, IRQ enabled
  sta ACIA_CMD

; Prints test_str
  ldx 0
print:
  lda test_str,x
  pha
  jsr print_acia
  inx
  pla
  bne print

; //////////////////////////////////////////////////////////////////////////////////////////////////
; MAIN LOOP
; Wait until a /r character is in the head of the buffer.
; //////////////////////////////////////////////////////////////////////////////////////////////////

wait_input:           
  lda BUFF_PRINT      ; Check if there's something to print.
  cmp BUFF_HEAD
  beq check_new_line
  tax                 ; Head and print index aren't the same, there's something to print.
  lda BUFF,x
  jsr print_acia
  inc BUFF_PRINT

check_new_line:
  lda BUFF_HEAD       ; Check if the character at head is /r.
  clc
  adc #$ff
  tax
  lda BUFF,x
  cmp #$D
  bne wait_input

; //////////////////////////////////////////////////////////////////////////////////////////////////
; EXIT
; Prints the content of the buffer.
; //////////////////////////////////////////////////////////////////////////////////////////////////

  lda #10            ; Prints a new line character
  jsr print_acia
  lda #'>'            ; Prints a > character
  jsr print_acia
  lda #' '            ; Prints an space character
  jsr print_acia

print_buffer:
  jsr buff_len
  beq exit            ; If the buffer lenght is zero, we're done.

  lda BUFF_TAIL
  tax
  lda BUFF,x          ; Read the character at tail index.
  jsr print_acia      ; Print it.

  inc BUFF_TAIL       ; Increment the tail index.
  jmp print_buffer

exit:
  brk   ; End of program

buff_len:         ; Stores in A the length of the circular buffer BUFF.
  lda BUFF_HEAD
  sec
  sbc BUFF_TAIL
  rts

print_acia:       ; Prints a character loaded in A.
  sta ACIA_DATA     ; Send
wait_for_clear:
  lda ACIA_STATUS
  and #$10
  beq wait_for_clear  ; Wait until the ACIA has send the byte.
  rts

irq:
  pha
  txa
  pha
  lda ACIA_STATUS       ; Check if the ACIA is the one triggering the IRQ.
  bmi process_acia_irq  ; Use the 7th bit of the STATUS register to get if there's an interrupt.
exit_irq: 
  pla
  tax
  pla
  rti

process_acia_irq:
  and #$08          ; Is there a new RX?
  beq exit_irq      ; If not, exit the IRQ.
  lda BUFF_HEAD
  tax
  lda ACIA_DATA     ; Get the RX byte.
  sta BUFF,x        ; Store it at BUFF_HEAD
  inc BUFF_HEAD     ; Increment BUFF_HEAD
  jmp exit_irq

  .org $fffc
  .word reset
  .word irq