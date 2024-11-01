; **************************************************************************************************
; sieve.s
; 
; Eratosthenes sieve up until 255. 
; This program will load in Y at the end all prime numbers up to 255.
; **************************************************************************************************

; Math reserved area.
OP1 = 0
OP2 = 2
RESULT = 4    ; 4 BYTES
AUX = 8       ; 16 BIT ACCUMULATOR

; Table variables area.
TABLE = $200

    .org $8000
reset:
  SEI				  
  LDX #$FF  ; INITIALIZING THE STACK POINTER
	TXS
	CLD       ; Clear decimal mode.

  JSR sieve

  ; CHECK THE TABLE. LOAD THE NUMBERS THAT ARE NOT CROSSED TO Y.
  LDX #0
print_loop:
  LDA TABLE,X
  BNE next_print_loop ; IF THE NUMBER OTHER THAN ZERO, THE NUMBER HAS BEEN CROSSED.
  TXA
  TAY
next_print_loop:
  INX
  BNE print_loop

  BRK   ; End of program.

; **************************************************************************************************
; SIEVE ALGORITHM
; **************************************************************************************************

sieve:
  ; Set to 0 all memory points in the table.
  LDA #0
  TAX
reset_table:
  STA TABLE,X
  DEX
  BNE reset_table

  ; X will be the testing index: 2, 3, 4, 5...
  LDX #2
sieve_loop:
  LDA TABLE,X
  BNE next_index  ; If NOT 0, the number has been crossed, if 0, that number is prime.

  TXA
  STA OP1
new_multiple:
  TXA
  STA OP2
  LDA #0
  STA OP1+1     ; THESE COULD BE SAVED...
  STA OP2+1
  STA RESULT
  STA RESULT+1

  JSR sum_u16     ; Get the next multiple of the number.
  
  ; Get if the multiple is greater than 255.
  LDA RESULT+1
  BNE next_index  ; ENOUGHT MULTIPLES FOUND, SKIP TO THE NEXT X.

  ; TEMPORARY SAVE OF X IN Y.
  TXA
  TAY
  ; CROSS THE NUMBER ON THE SIEVE BY PUTTING A ONE THERE.
  LDA RESULT
  TAX
  INC TABLE,X
  ; RESTORE X.
  TYA
  TAX

  LDA RESULT
  STA OP1
  JMP new_multiple
    
next_index:
  INX
  BNE sieve_loop 
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

  .org $fffc
  .word reset
  .word $0000