; **************************************************************************************************
; acia_hw_ctrl.s
; Prints a hello message and awaits user input. If a lot of text is pasted at once, the 6502 will 
; trigger the flow control signals of the ACIA to stop the pasting from the PC until the buffer 
; is empty enough. Once the user presses enter, the program ends.
; **************************************************************************************************

ACIA_DATA   = $A000
ACIA_STATUS = $A001
ACIA_CMD    = $A002
ACIA_CTRL   = $A003

OP1         = 0
OP2         = 2
RESULT      = 4     ; 4 BYTES
AUX         = 8     ; 16 BIT ACCUMULATOR

BUFF        = $0200   ; Input buffer of 256 bytes.
BUFF_HEAD   = $20     ; Write position
BUFF_TAIL   = $21     ; Read position
BUFF_PRINT  = $22     ; Position of input buffer that's been printed.

PRINT_STRING_ADDRS = $28  ; Address of the string to be printed by print_str.

CALC_OPERATION    = $30   ; Stores the operation. 0 if no operation is present.
CALC_OP_DIRECTION = $31   ; Stores the direction where the operand should be stored (2 bytes)
CALC_OP1          = $34
CALC_OP2          = $36
CALC_RESULT       = $3A

  .org $E000
error_str:
  .text "ERR"
  .byte 0
overflow_str:
  .text "OVERFLOW"
  .byte 0

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

new_input:
  ldx #$FF              ; Reset the stack pointer.
  txs

  lda #$10             ; Erase the 16 bit area of the CALCULATOR zp.
  tax
  lda #0
erase_calculator_area:
  dex
  sta CALC_OPERATION,x
  bne erase_calculator_area

  lda #$0D           ; Carriage return
  jsr print_acia
  lda #$0A           ; New line
  jsr print_acia
  lda #'>'
  jsr print_acia

wait_input:           
  lda BUFF_HEAD
  cmp BUFF_PRINT
  beq wait_input      ; If there's no new input, wait here.

  cmp #200            ; Check if there are more than 200 characters without printing.
  bcs activate_rts
  cmp #10             ; Check if there less than 10 characters without printing.
  bcc deacivate_rts
  jmp print_new_char

activate_rts:
  lda ACIA_CMD
  ora #%00001000      ; Activate the RTS
  sta ACIA_CMD
  jmp print_new_char

deacivate_rts:
  lda ACIA_CMD
  and #%11110111      ; Deactivate the RTS
  sta ACIA_CMD

print_new_char:
  ldx BUFF_PRINT      ; Head and print index aren't the same, there's something to print.
  lda BUFF,x
  pha
  jsr print_acia
  inc BUFF_PRINT
  pla

  cmp #'+'            ; Check if the input is an operation.
  beq set_operation
  cmp #'-'
  beq set_operation
  cmp #'*'
  beq set_operation
  cmp #'/'
  beq set_operation

check_enter_press:
  cmp #$D             ; Compare if the current input is /r  => End of program.

  bne wait_input

exit:
  brk                 ; End of program

set_operation:
  pha
  lda CALC_OPERATION
  bne print_error           ; Two operation symbols have been inserted! => ERROR
  pla
  sta CALC_OPERATION        ; Save the operation.

  lda #<CALC_OP1            ; Put the first operand in the CALC_OP_DIRECTION.
  sta CALC_OP_DIRECTION
  lda #>CALC_OP1
  sta CALC_OP_DIRECTION+1
  jsr parse_number          ; Parse the number.
  jmp wait_input

parse_number:
  ; The number is between the PRINT and TAIL, where PRINT is the less significant digit.
  ; BUFF_PRINT is currently pointing to the next position after operator (+,-,*,/) or the = sign.
  lda BUFF_PRINT
  dec A                     ; Now it's on the operator.
  dec A                     ; Now it's on the number.
  pha

parse_number_next_digit:
  pla
  cmp BUFF_TAIL             ; If A < TAIL, the number has been parsed. 
  bmi parse_number_return
  pha                       ; Put the index of the digit on the stack.
  
  ldy #0
  lda (CALC_OP_DIRECTION),y   ; Multiply the current number by 10.
  sta OP1
  iny
  lda (CALC_OP_DIRECTION),y
  sta OP1+1
  lda #10
  sta OP2
  lda #0
  sta OP2+1
  jsr mult_u16

  lda RESULT+2              ; Check if the number is getting larger than 16 bits.
  bne print_overflow

  lda RESULT                ; If not, store it so that it can be added the next digit.
  ldy #0
  sta (CALC_OP_DIRECTION),y
  sta OP1                   ; Now that it's loaded, let's put it in OP1 for the latter sum.
  lda RESULT+1
  iny
  sta (CALC_OP_DIRECTION),y
  sta OP1+1

  pla                       ; Restore the index of the digit from the stack.
  tax
  lda BUFF,x
  cmp #'0'                  ; Check if the digit is valid.
  bcc print_error           ; If less than zero, print error and go back to the start.
  cmp #':'                  ; : is the next character after '9'
  bcs print_error

  tay
  dex                       ; Decrement the index and save it for the next iteration.
  txa                       ; Substitute all this by dex and phx
  pha
  tya

  sec
  sbc #'0'                  ; Convert the digit from ASCII to integer

  sta OP2                   ; Store the digit on OP2.
  lda #0
  sta OP2+1
  jsr sum_u16               ; Sum the digit.

  lda RESULT+2              ; Check if the number is getting larger than 16 bits.
  bne print_overflow

  lda RESULT                ; If not, store it.
  ldy #0
  sta (CALC_OP_DIRECTION),y
  lda RESULT+1
  iny
  sta (CALC_OP_DIRECTION),y

  jmp parse_number_next_digit
parse_number_return:
  ldy #0
  lda (CALC_OP_DIRECTION),y
  iny
  lda (CALC_OP_DIRECTION),y

  brk
  rts

print_error:                ; PRINT_STRING_ADDRS = error_str and print it.
  lda #<error_str
  sta PRINT_STRING_ADDRS
  lda #>error_str
  sta PRINT_STRING_ADDRS+1
  jsr print_str
  jmp new_input

print_overflow:                ; PRINT_STRING_ADDRS = overflow_str and print it.
  lda #<overflow_str
  sta PRINT_STRING_ADDRS
  lda #>overflow_str
  sta PRINT_STRING_ADDRS+1
  jsr print_str
  jmp new_input

print_str:
  ldy #0
print_str_loop:
  lda (PRINT_STRING_ADDRS),y
  pha
  jsr print_acia
  iny
  pla
  bne print_str_loop
  rts

print_acia:       ; Prints a character loaded in A.
  sta ACIA_DATA     ; Send
wait_for_clear:
  lda ACIA_STATUS
  and #$10
  beq wait_for_clear  ; Wait until the ACIA has send the byte.
  rts

; **************************************************************************************************
; MATH OPERATIONS
; **************************************************************************************************
clear_results_region:
  lda #0
  sta RESULT
  sta RESULT+1
  sta RESULT+2
  sta RESULT+3
  sta AUX
  sta AUX+1
  rts

sum_u16:
  jsr clear_results_region
  CLC
  LDA OP1
  ADC OP2
  STA RESULT
  LDA OP1+1
  ADC OP2+1
  STA RESULT+1
  RTS

sub_u16:
  jsr clear_results_region
  SEC
  LDA OP1
  SBC OP2
  STA RESULT
  LDA OP1+1
  SBC OP2+1
  STA RESULT+1
  RTS

mult_u16:
  LDX #16
L1_u16:
  LSR OP2+1
  ROR OP2      ; Get low bit of OP2
  BCC L2_u16   ; 0 or 1?
  LDA AUX      ; If 1, add to the "summing area" in AUX
  CLC
  ADC OP1      
  STA AUX          
  LDA AUX+1
  ADC OP1+1
L2_u16:    
  ROR A       ; Pass the carry from the previous ADC to the sumand.
  STA AUX+1   ; Save the MSB of the sumand.
  ROR AUX
  ROR RESULT+1
  ROR RESULT
  DEX
  BNE L1_u16

  LDA AUX+1
  STA RESULT+3
  LDA AUX
  STA RESULT+2
  RTS

div_u16:
  jsr clear_results_region
  ; REMAINDER: LSB IN RESULT +2, MSB IN RESULT + 3
  LDX #16
test_div_u16:
  ASL RESULT    ; Move the result to the left and INC if remainder >= OP2.
  ROL RESULT+1
  ASL OP1
  ROL OP1+1
  ROL RESULT+2  ; Charge a bit into the remainder.
  ROL RESULT+3

  LDA RESULT+2  ; Try to subtract the divisor to the remainder.
  SEC
  SBC OP2
  TAY           ; SAVE THE LSB OF THE RESULT IN Y
  LDA RESULT+3
  SBC OP2+1

  ; NOW CHECK IF THE NUMBER IN AUX IS POSITIVE OR ZERO.
  ; AN ABSOLUTE NEGATIVE RESULT IN SBC WILL ALWAYS OUTPUT C=0
  BCC next_div_u16

  ; if reached here, subtraction was either zero or positive.
  STA RESULT+3  ; If positive, store the result of the subtraction in the remainder.
  TYA
  STA RESULT+2

  INC RESULT    ; Put a one on the LSB of the result.
next_div_u16:
  DEX
  BNE test_div_u16
  RTS

; **************************************************************************************************
; IRQ
; **************************************************************************************************
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