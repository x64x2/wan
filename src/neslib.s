;NES hardware-dependent functions

	.export _pal_all,_pal_bg,_pal_spr,_pal_col,_pal_clear,_pal_bright
	.export _ppu_off,_ppu_on_all,_ppu_on_bg,_ppu_on_spr,_ppu_mask, _ppu_system
	.export _oam_clear,_oam_size,_oam_spr,_oam_meta_spr,_oam_hide_rest
	.export _ppu_waitnmi
	.export _unrle_vram
	.export _scroll, _split
	.export _bank_spr,_bank_bg
	.export _vram_read,_vram_write
	.export _music_play,_music_stop,_music_pause
	.export _sfx_play
	.export _pad_poll,_pad_trigger,_pad_state
	.export _rand8,_rand16,_set_rand
	.export _set_vram_update,_vram_adr,_vram_put,_vram_fill,_vram_inc
	.export _memcpy,_memfill,_delay
	.export _bankswitch



;NMI handler

nmi:
	pha
	txa
	pha
	tya
	pha

	lda <PPU_MASK_VAR	;if rendering is disabled, do not access the VRAM at all
	and #%00011000
	beq @skip
	lda <VRAMUPDATE
	bne @upd
	jmp @skipUpd
@skip:
	jmp @skipAll
	
@upd:
	ldx #$00
	stx PPU_OAM_ADDR
	lda #>OAM_BUF
	sta PPU_OAM_DMA

	lda <PALUPDATE
	bne @palupd
	jmp @upd1
@palupd:

	stx <PALUPDATE

	lda #$3f
	sta PPU_ADDR
	stx PPU_ADDR

	.repeat 4,I
	ldy PAL_BUF+I
	lda (PAL_PTR),y
	sta PPU_DATA
	.endrepeat

	.repeat 7,J
	lda PPU_DATA			;skip background color
	.repeat 3,I
	ldy PAL_BUF+5+(J*4)+I
	lda (PAL_PTR),y
	sta PPU_DATA
	.endrepeat
	.endrepeat

@upd1:

	ldx <NAME_UPD_LEN
	beq @skipUpd
	ldy #0
@updName:
	lda (NAME_UPD_ADR),y
	iny
	sta PPU_ADDR
	lda (NAME_UPD_ADR),y
	iny
	sta PPU_ADDR
	lda (NAME_UPD_ADR),y
	iny
	sta PPU_DATA
	dex
	bne @updName

@skipUpd:
	stx PPU_ADDR
	stx PPU_ADDR

	lda <SCROLL_X
	sta PPU_SCROLL
	lda <SCROLL_Y
	sta PPU_SCROLL
	lda <PPU_CTRL_VAR
	sta PPU_CTRL

@skipAll:
	inc <FRAMECNT1
	inc <FRAMECNT2
	lda <FRAMECNT2
	cmp #6
	bne @skipNtsc
	lda #0
	sta <FRAMECNT2
@skipNtsc:

	jsr FamiToneUpdate

	pla
	tay
	pla
	tax
	pla

irq:
    rti



;void __fastcall__ pal_all(const char *data);

_pal_all:
	sta <PTR
	stx <PTR+1
	ldx #$00
	lda #$20

pal_copy:
	sta <LEN

	ldy #$00
@0:
	lda (PTR),y
	sta PAL_BUF,x
	inx
	iny
	dec <LEN
	bne @0
	
	inc <PALUPDATE

	rts



;void __fastcall__ pal_bg(const char *data);

_pal_bg:
	sta <PTR
	stx <PTR+1
	ldx #$00
	lda #$10
	bne pal_copy ;bra



;void __fastcall__ pal_spr(const char *data);

_pal_spr:
	sta <PTR
	stx <PTR+1
	ldx #$10
	txa
	bne pal_copy ;bra



;void __fastcall__ pal_col(unsigned char index,unsigned char color);

_pal_col:
	sta <PTR
	jsr popa
	and #$1f
	tax
	lda <PTR
	sta PAL_BUF,x
	inc <PALUPDATE
	rts



;void __fastcall__ pal_clear(void);

_pal_clear:
;	lda #$0f
;	ldx #0
;@1:
;	sta PAL_BUF,x
;	inx
;	cpx #$20
;	bne @1
;	stx <PALUPDATE
	rts



;void __fastcall__ pal_bright(unsigned char bright);

_pal_bright:
	tax
	lda palBrightTableL,x
	sta <PAL_PTR
	lda palBrightTableH,x	;MSB is never zero
	sta <PAL_PTR+1
	sta <PALUPDATE
	rts

;void __fastcall__ ppu_off(void);

_ppu_off:
	lda <PPU_MASK_VAR
	and #%00011000
	beq @1
	jsr _ppu_waitnmi
@1:
	lda <PPU_MASK_VAR
	and #%11100111
	sta <PPU_MASK_VAR
	sta PPU_MASK
	rts

;unsigned char __fastcall__ ppu_system(void);

_ppu_system:
	lda <NTSCMODE
	rts

;void __fastcall__ ppu_on_all(void);

_ppu_on_all:
	lda <PPU_MASK_VAR
	ora #%00011000
ppu_onoff:
	sta <PPU_MASK_VAR
	sta PPU_MASK
	jsr _ppu_waitnmi
	lda #$00
	sta PPU_ADDR
	sta PPU_ADDR
	lda <PPU_CTRL_VAR
	sta PPU_CTRL
	rts



;void __fastcall__ ppu_on_bg(void);

_ppu_on_bg:
	lda <PPU_MASK_VAR
	ora #%00001000
	bne ppu_onoff	;bra



;void __fastcall__ ppu_on_spr(void);

_ppu_on_spr:
	lda <PPU_MASK_VAR
	ora #%00010000
	bne ppu_onoff	;bra



;void __fastcall__ ppu_mask(unsigned char mask);

_ppu_mask:
	sta <PPU_MASK_VAR
	sta PPU_MASK
	rts



;void __fastcall__ oam_clear(void);

_oam_clear:
	ldx #0
	lda #$ff
@1:
	sta OAM_BUF,x
	inx
	inx
	inx
	inx
	bne @1
	rts



;void __fastcall__ oam_size(unsigned char size);

_oam_size:
	asl a
	asl a
	asl a
	asl a
	asl a
	and #$20
	sta <TEMP
	lda <PPU_CTRL_VAR
	and #$df
	ora <TEMP
	sta <PPU_CTRL_VAR
	rts
	
	
	
;unsigned char __fastcall__ oam_spr(unsigned char x,unsigned char y,unsigned char chrnum,unsigned char attr,unsigned char sprid);

_oam_spr:
	tax

	ldy #0		;four popa calls replacement
	lda (sp),y
	iny
	sta OAM_BUF+2,x
	lda (sp),y
	iny
	sta OAM_BUF+1,x
	lda (sp),y
	iny
	sta OAM_BUF+0,x
	lda (sp),y
	sta OAM_BUF+3,x

	lda <sp
	clc
	adc #4
	sta <sp
	bcc @1
	inc <sp+1
@1:

	txa
	clc
	adc #4
	rts



;unsigned char __fastcall__ oam_meta_spr(unsigned char x,unsigned char y,unsigned char sprid,const unsigned char *data);

_oam_meta_spr:
	sta <PTR
	stx <PTR+1

	ldy #2		;three popa calls replacement, performed in reversed order
	lda (sp),y
	dey
	sta <SCRX
	lda (sp),y
	dey
	sta <SCRY
	lda (sp),y
	tax

@1:
	lda (PTR),y		;x offset
	cmp #$80
	beq @2
	iny
	clc
	adc <SCRX
	sta OAM_BUF+3,x
	lda (PTR),y		;y offset
	iny
	clc
	adc <SCRY
	sta OAM_BUF+0,x
	lda (PTR),y		;tile
	iny
	sta OAM_BUF+1,x
	lda (PTR),y		;attribute
	iny
	sta OAM_BUF+2,x
	inx
	inx
	inx
	inx
	jmp @1
@2:

	lda <sp
	adc #2			;carry is always set here, so it adds 3
	sta <sp
	bcc @3
	inc <sp+1
@3:

	txa
	rts



;void __fastcall__ oam_hide_rest(unsigned char sprid);

_oam_hide_rest:
	tax
	lda #240
@1:
	sta OAM_BUF,x
	inx
	inx
	inx
	inx
	bne @1
	rts



;void __fastcall__ ppu_waitnmi(void);

_ppu_waitnmi:
	lda #1
	sta <VRAMUPDATE
	lda <FRAMECNT1
@1:
	cmp <FRAMECNT1
	beq @1
	lda <NTSCMODE
	beq @3
@2:
	lda <FRAMECNT2
	cmp #5
	beq @2
@3:
	lda #0
	sta <VRAMUPDATE
	rts



;void __fastcall__ unrle_vram(const unsigned char *data,unsigned int vram);

_unrle_vram:
	stx PPU_ADDR
	sta PPU_ADDR

	jsr popax
	tay
	stx <RLE_HIGH
	lda #0
	sta <RLE_LOW

	lda (RLE_LOW),y
	sta <RLE_TAG
	iny
	bne @1
	inc <RLE_HIGH
@1:
	lda (RLE_LOW),y
	iny
	bne @11
	inc <RLE_HIGH
@11:
	cmp <RLE_TAG
	beq @2
	sta PPU_DATA
	sta <RLE_BYTE
	bne @1
@2:
	lda (RLE_LOW),y
	beq @4
	iny
	bne @21
	inc <RLE_HIGH
@21:
	tax
	lda <RLE_BYTE
@3:
	sta PPU_DATA
	dex
	bne @3
	beq @1
@4:
	rts



;void __fastcall__ scroll(unsigned int x,unsigned int y);

_scroll:
	sta <TEMP

	txa
	bne @1
	lda <TEMP
	cmp #240
	bcs @1
	sta <SCROLL_Y
	lda #0
	sta <TEMP
	beq @2	;bra

@1:
	sec
	lda <TEMP
	sbc #240
	sta <SCROLL_Y
	lda #2
	sta <TEMP
@2:

	jsr popax
	sta <SCROLL_X
	txa
	and #$01
	ora <TEMP
	sta <TEMP
	lda <PPU_CTRL_VAR
	and #$fc
	ora <TEMP
	sta <PPU_CTRL_VAR
	rts

;;void __fastcall__ split(unsigned int x,unsigned int y);

_split:
	jsr popax
	sta <SCROLL_X1
	txa
	and #$01
	sta <TEMP
	lda <PPU_CTRL_VAR
	and #$fc
	ora <TEMP
	sta <PPU_CTRL_VAR1

@3:
	bit PPU_STATUS
	bvs @3
@4:
	bit PPU_STATUS
	bvc @4

	lda <SCROLL_X1
	sta PPU_SCROLL
	lda #0
	sta PPU_SCROLL
	lda <PPU_CTRL_VAR1
	sta PPU_CTRL

	rts


;void __fastcall__ bank_spr(unsigned char n);

_bank_spr:
	and #$01
	asl a
	asl a
	asl a
	sta <TEMP
	lda <PPU_CTRL_VAR
	and #%11110111
	ora <TEMP
	sta <PPU_CTRL_VAR
	rts



;void __fastcall__ bank_bg(unsigned char n);

_bank_bg:
	and #$01
	asl a
	asl a
	asl a
	asl a
	sta <TEMP
	lda <PPU_CTRL_VAR
	and #%11101111
	ora <TEMP
	sta <PPU_CTRL_VAR
	rts



;void __fastcall__ vram_read(unsigned char *dst,unsigned int adr,unsigned int size);

_vram_read:
	sta <TEMP
	stx <TEMP+1

	jsr popax
	stx PPU_ADDR
	sta PPU_ADDR
	lda PPU_DATA

	jsr popax
	sta <TEMP+2
	stx <TEMP+3

	ldy #0
@1:
	lda PPU_DATA
	sta (TEMP+2),y
	inc <TEMP+2
	bne @2
	inc <TEMP+3
@2:
	lda <TEMP
	bne @3
	dec <TEMP+1
@3:
	dec <TEMP
	lda <TEMP
	ora <TEMP+1
	bne @1

	rts



;void __fastcall__ vram_write(unsigned char *src,unsigned int adr,unsigned int size);

_vram_write:
	sta <TEMP
	stx <TEMP+1

	jsr popax
	stx PPU_ADDR
	sta PPU_ADDR

	jsr popax
	sta <TEMP+2
	stx <TEMP+3

	ldy #0
@1:
	lda (TEMP+2),y
	sta PPU_DATA
	inc <TEMP+2
	bne @2
	inc <TEMP+3
@2:
	lda <TEMP
	bne @3
	dec <TEMP+1
@3:
	dec <TEMP
	lda <TEMP
	ora <TEMP+1
	bne @1

	rts



;void __fastcall__ music_play(unsigned char song);

_music_play=FamiToneMusicPlay



;void __fastcall__ music_stop(void);

_music_stop=FamiToneMusicStop



;void __fastcall__ music_pause(unsigned char pause);

_music_pause=FamiToneMusicPause



;void __fastcall__ sfx_play(unsigned char sound,unsigned char channel);

_sfx_play:
	and #$03
	tax
	lda @sfxPriority,x
	tax
	jsr popa
	jmp FamiToneSfxPlay

@sfxPriority:
	.byte FT_SFX_CH0,FT_SFX_CH1,FT_SFX_CH2,FT_SFX_CH3



;unsigned char __fastcall__ pad_poll(unsigned char pad);

_pad_poll:
	tay
	ldx #0

@padPollPort:
	lda #1
	sta CTRL_PORT1
	lda #0
	sta CTRL_PORT1
	lda #8
	sta <TEMP

@padPollLoop:
	lda CTRL_PORT1,y
	lsr a
	ror <PAD_BUF,x
	dec <TEMP
	bne @padPollLoop

	inx
	cpx #3
	bne @padPollPort

	lda <PAD_BUF
	cmp <PAD_BUF+1
	beq @done
	cmp <PAD_BUF+2
	beq @done
	lda <PAD_BUF+1
@done:
	sta <PAD_STATE,y
	tax
	eor <PAD_STATEP,y
	and <PAD_STATE ,y
	sta <PAD_STATET,y
	txa
	sta <PAD_STATEP,y

	rts



;unsigned char __fastcall__ pad_trigger(unsigned char pad);

_pad_trigger:
	pha
	jsr _pad_poll
	pla
	tax
	lda <PAD_STATET,x
	rts



;unsigned char __fastcall__ pad_state(unsigned char pad);

_pad_state:
	tax
	lda <PAD_STATE,x
	rts



;unsigned char __fastcall__ rand8(void);
;Galois random generator, found somewhere
;out: A random number 0..255

rand1:
	lda <RAND_SEED
	asl a
	bcc @1
	eor #$cf
@1:
	sta <RAND_SEED
	rts

rand2:
	lda <RAND_SEED+1
	asl a
	bcc @1
	eor #$d7
@1:
	sta <RAND_SEED+1
	rts

_rand8:
	jsr rand1
	jsr rand2
	adc <RAND_SEED
	rts



;unsigned int __fastcall__ rand16(void);

_rand16:
	jsr rand1
	tax
	jsr rand2
	rts


;void __fastcall__ set_rand(unsigned char seed);

_set_rand:
	sta <RAND_SEED
	stx <RAND_SEED+1
	rts



;void __fastcall__ set_vram_update(unsigned char len,unsigned char *buf);

_set_vram_update:
	sta <NAME_UPD_ADR
	stx <NAME_UPD_ADR+1
	jsr popa
	sta <NAME_UPD_LEN
	rts



;void __fastcall__ vram_adr(unsigned int adr);

_vram_adr:
	stx PPU_ADDR
	sta PPU_ADDR
	rts



;void __fastcall__ vram_put(unsigned char n);

_vram_put:
	sta PPU_DATA
	rts



;void __fastcall__ vram_fill(unsigned char n,unsigned int len);

_vram_fill:
	sta <LEN
	stx <LEN+1
	jsr popa
	ldx <LEN+1
	beq @2
	ldx #0
@1:
	sta PPU_DATA
	dex
	bne @1
	dec <LEN+1
	bne @1
@2:
	ldx <LEN
	beq @4
@3:
	sta PPU_DATA
	dex
	bne @3
@4:
	rts



;void __fastcall__ vram_inc(unsigned char n);

_vram_inc:
	beq @1
	lda #$04
@1:
	sta <TEMP
	lda <PPU_CTRL_VAR
	and #$fb
	ora <TEMP
	sta <PPU_CTRL_VAR
	sta PPU_CTRL
	rts



;void __fastcall__ memcpy(void *dst,void *src,unsigned int len);

_memcpy:
	sta <LEN
	stx <LEN+1
	jsr popax
	sta <SRC
	stx <SRC+1
	jsr popax
	sta <DST
	stx <DST+1

	ldx #0
@1:
	lda <LEN+1
	beq @2
	jsr @3
	dec <LEN+1
	inc <SRC+1
	inc <DST+1
	jmp @1
@2:
	ldx <LEN
	beq @5

@3:
	ldy #0
@4:
	lda (SRC),y
	sta (DST),y
	iny
	dex
	bne @4
@5:
	rts


	
;void __fastcall__ memfill(void *dst,unsigned char value,unsigned int len);

_memfill:
	sta <LEN
	stx <LEN+1
	jsr popa
	sta <TEMP
	jsr popax
	sta <DST
	stx <DST+1

	ldx #0
@1:
	lda <LEN+1
	beq @2
	jsr @3
	dec <LEN+1
	inc <DST+1
	jmp @1
@2:
	ldx <LEN
	beq @5

@3:
	ldy #0
	lda <TEMP
@4:
	sta (DST),y
	iny
	dex
	bne @4
@5:
	rts
	
	
	
;void __fastcall__ delay(unsigned char frames);

_delay:
	tax
@1:
	jsr _ppu_waitnmi
	dex
	bne @1
	rts

; void __fastcall__ bankswitch(unsigned char bank);

_bankswitch:
	tax
	sta bankList, x
	rts

bankList:
	.byte $00, $01, $02, $03
	
palBrightTableL:
	.byte <palBrightTable0,<palBrightTable1,<palBrightTable2
	.byte <palBrightTable3,<palBrightTable4,<palBrightTable5
	.byte <palBrightTable6,<palBrightTable7,<palBrightTable8
palBrightTableH:
	.byte >palBrightTable0,>palBrightTable1,>palBrightTable2
	.byte >palBrightTable3,>palBrightTable4,>palBrightTable5
	.byte >palBrightTable6,>palBrightTable7,>palBrightTable8

palBrightTable0:
	.byte $0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f	;black
palBrightTable1:
	.byte $0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f
palBrightTable2:
	.byte $0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f
palBrightTable3:
	.byte $0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f
palBrightTable4:
	.byte $00,$01,$02,$03,$04,$05,$06,$07,$08,$09,$0a,$0b,$0c,$0f,$0f,$0f	;normal colors
palBrightTable5:
	.byte $10,$11,$12,$13,$14,$15,$16,$17,$18,$19,$1a,$1b,$1c,$1d,$00,$00
palBrightTable6:
	.byte $10,$21,$22,$23,$24,$25,$26,$27,$28,$29,$2a,$2b,$2c,$2d,$10,$10	;$10 because $20 is the same as $30
palBrightTable7:
	.byte $30,$31,$32,$33,$34,$35,$36,$37,$38,$39,$3a,$3b,$3c,$3d,$20,$20
palBrightTable8:
	.byte $30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30	;white
	.byte $30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30
	.byte $30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30
	.byte $30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30

	.include "famitone2.s"
	