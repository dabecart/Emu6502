; **************************************************************************************************
; u8Math.s
; 
; Simple program with utilities to do basic byte math. The result of the operations will be stored 
; at RESULT.
;   - Sum and sub operations can be used for unsigned and signed operations. 
;   - Mult and div only work with absolute terms.
; **************************************************************************************************

OP1_VAL = 255
OP2_VAL = 253

  .org $8000

OP1 = 0
OP2 = 1
RESULT = 2

reset:
  SEI				  
  LDX #$FF  ; INITIALIZING THE STACK POINTER
	TXS
	CLD       ; Clear decimal mode.

  LDA #OP1_VAL
  STA OP1
  LDA #OP2_VAL
  STA OP2

  JSR div_u8

  LDA RESULT
  LDA RESULT+1

  BRK   ; End of program.

; **************************************************************************************************
; MATH OPERATIONS
; **************************************************************************************************
sum_u8:
  CLC
  LDA OP1
  ADC OP2
  STA RESULT
  RTS

sub_u8:
  SEC
  LDA OP1
  SBC OP2
  STA RESULT
  RTS

mult_u8:
  LDA #$80     ;Preload sentinel bit into RESULT
  STA RESULT
  ASL A        ;Initialize RESULT hi byte to 0
	DEC OP1
L1:
  LSR OP2      ;Get low bit of OP2
  BCC L2       ;0 or 1?
  ADC OP1      ;If 1, add (OP1-1)+1
L2:     
  ROR A        ;"Stairstep" shift (catching carry from add)
  ROR RESULT
  BCC L1       ;When sentinel falls off into carry, we're done
  STA RESULT+1
  RTS

div_u8:
  LDX #8
test_div:
  ASL RESULT    ; Move the result to the left and INC if remainder >= OP2.
  ASL OP1
  ROL RESULT+1  ; Charge a bit into the remainder.
  LDA RESULT+1  ; Try to subtract the divisor to the remainder.
  SEC
  SBC OP2
  BCC next_div    ; If negative (after subtraction, if carry is 0, result is negative in absolute terms), go to the next bit.
add_rem_div:    ; if reached here, subtraction was either zero or positive.
  STA RESULT+1  ; If positive, store the result of the subtraction in the remainder.
  INC RESULT    ; Put a one on the LSB of the result.
next_div:
  DEX
  BNE test_div
  RTS

mod_u8:
  JSR div_u8    ; Move the remainder to the result field.
  LDA RESULT+1
  STA RESULT
  RTS

  .org $fffc
  .word reset
  .word $0000