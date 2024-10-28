  .org $8000
reset:
  lda $ffff
  lda $ffff,x
  lda $ffff,y
  lda #$ff
  
  lda $ff
  lda ($ff,x)
  lda $ff,x
  ldx $ff,y
  lda ($ff),y

  .org $fffc
  .word reset
  .word $0000