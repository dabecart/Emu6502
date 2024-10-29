; **************************************************************************************************
; u16Math.s
; 
; Simple program with utilities to do basic word math. The result of the operations will be stored 
; at RESULT. 
; Operations are similar to u8 but for multiplication and division a 16 bit accumulator is needed. 
; For that we got AUX. 
;   - Sum and sub operations can be used for unsigned and signed operations. 
;   - Mult and div only work with absolute terms.
; **************************************************************************************************

OP1_VAL_L = $5a
OP1_VAL_H = $04
OP2_VAL_L = $03
OP2_VAL_H = $01

  .org $8000

OP1 = 0
OP2 = 2
RESULT = 4    ; 4 BYTES
AUX = 8       ; 16 BIT ACCUMULATOR

reset:
  SEI				  
  LDX #$FF  ; INITIALIZING THE STACK POINTER
	TXS
	CLD       ; Clear decimal mode.

  JSR set_vals
  JSR sum_u16
  LDA RESULT
  LDA RESULT+1

  NOP

  JSR set_vals
  JSR sub_u16
  LDA RESULT
  LDA RESULT+1

  NOP

  JSR set_vals
  JSR mult_u16
  LDA RESULT
  LDA RESULT+1
  LDA RESULT+2
  LDA RESULT+3

  NOP

  JSR set_vals
  JSR div_u16
  LDA RESULT
  LDA RESULT+1
  LDA RESULT+2
  LDA RESULT+3

  BRK   ; End of program.

set_vals:
  LDA #OP1_VAL_L
  STA OP1
  LDA #OP1_VAL_H
  STA OP1+1
  LDA #OP2_VAL_L
  STA OP2
  LDA #OP2_VAL_H
  STA OP2+1

  LDA #0
  STA RESULT
  STA RESULT+1
  STA RESULT+2
  STA RESULT+3
  STA AUX
  STA AUX+1

  RTS

; **************************************************************************************************
; MATH OPERATIONS
; **************************************************************************************************
sum_u16:
  CLC
  LDA OP1
  ADC OP2
  STA RESULT
  LDA OP1+1
  ADC OP2+1
  STA RESULT+1
  RTS

sub_u16:
  SEC
  LDA OP1
  SBC OP2
  STA RESULT
  LDA OP1+1
  SBC OP2+1
  STA RESULT+1
  RTS

mult_u16:
  LDA #$80      ; Preload sentinel bit. It will travel 16 positions until it gets in the C and done!
  STA RESULT+1
  LDA OP1
  BNE L0_u16
  DEC OP1+1     ; Subtract 1 from OP1 so that non CLC has to be used on the ADC call (nifty).
L0_u16:
	DEC OP1

L1_u16:
  LSR OP2+1
  ROR OP2      ; Get low bit of OP2
  BCC L2_u16   ; 0 or 1?
  LDA AUX      ; If 1, add (OP1-1)+1
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
  BCC L1_u16   ; When sentinel falls off into carry, we're done

  LDA AUX+1
  STA RESULT+3
  LDA AUX
  STA RESULT+2
  RTS

div_u16:
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

mod_u16:
  JSR div_u16    ; Move the remainder to the result field.
  LDA RESULT+2
  STA RESULT
  LDA RESULT+3
  STA RESULT+1
  RTS

  .org $fffc
  .word reset
  .word $0000