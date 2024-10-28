  .org $8000
reset:
  lda #$ff
  sta $6002
  
  .org $fffc
  .word reset
  .word $0000