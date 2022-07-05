;NES 4-player controller reading - written for the ca65 assembler
;- works with FourScore on NES (and AV Famicom afaik)
;- works with Hori adapter on Famicom (up to 4 players)
;- works with expansion port controllers on Famicom (3 players)
;- should work with dual expansion port controllers on Famicom (4 players)
;- has checks in place to allow worn out standard controllers to be replaced by
;  expansion controllers on Famicom (depending on state of _playerActive array);
;  This is good behavior for Famicom, a lot of commercially released Famicom
;  games do the same!

.export _updateInput

;call this at the beginning of a frame
.proc _updateInput ;a,x,y
	
	;not 100% sure anymore why I wrote these out
	;most likely had to do with some zeropage reuse/optimization thing in Micro Mages
	byte2_4016_0 = buf4016_0 + 0
	byte1_4016_0 = buf4016_0 + 1
	byte0_4016_0 = buf4016_0 + 2
	byte2_4017_0 = buf4017_0 + 0
	byte1_4017_0 = buf4017_0 + 1
	byte0_4017_0 = buf4017_0 + 2
	
	byte2_4016_1 = buf4016_1 + 0
	byte1_4016_1 = buf4016_1 + 1
	byte0_4016_1 = buf4016_1 + 2
	byte2_4017_1 = buf4017_1 + 0
	byte1_4017_1 = buf4017_1 + 1
	byte0_4017_1 = buf4017_1 + 2
	
	;reset strobe bit
	ldy #$01
	sty $4016
	dey
	sty $4016
	
	;read joypad and famicom expansion pads as well as multitap adapters
	ldx #3-1
	@byteLoop:
		ldy #8
		@readLoop:
			lda $4016
			lsr a      ; bit0 -> Carry
			rol buf4016_0,x
			lsr a      ; bit1 -> Carry
			rol buf4016_1,x
			lda $4017
			lsr a      ; bit0 -> Carry
			rol buf4017_0,x
			lsr a      ; bit1 -> Carry
			rol buf4017_1,x
			dey
			bne @readLoop
		dex
		bpl @byteLoop
		
	lda byte0_4016_1
	sta _joy3
	lda byte0_4017_1
	sta _joy4
	
	;on Famicom, it is expected that the expansion port controller
	;can be used to replace a worn-out standard controller #1
	;Let's do that unless a third player joins the party
	lda byte0_4016_0
	ldy _playerActive+2 ;player 3
	beq :+
		ora _joy3
	:
	sta _joy1
	
	;also allow second expansion controller to replace standard controller #2
	lda byte0_4017_0
	ldy _playerActive+3 ;player 4
	beq :+
		ora _joy4
	:
	sta _joy2
	
	@detectMultitap:
		;check 3rd bytes from bit0 reads
		lda byte2_4016_0
		cmp #%00010000 ;$10
		bne @skipFourScore
			lda byte2_4017_0 ;$20
			cmp #%00100000
			bne @skipFourScore
				;FourScore detected
				;2nd bytes hold controller #3/#4 data
				lda byte1_4016_0
				ora _joy3
				sta _joy3
				lda byte1_4017_0
				ora _joy4
				sta _joy4
				jmp @skipDetectMultitap
		@skipFourScore:
		
		;check 3rd bytes from bit1 reads
		lda byte2_4016_1
		cmp #%00100000 ;$20
		bne @skipHori
			lda byte2_4017_1
			cmp #%00010000 ;$10
			bne @skipHori
				;hori adapter detected
				;2nd bytes hold controller #3/#4 data
				;allow to replace worn out standard famicom controllers with these as well (4p mode)
				lda byte1_4016_1
				ora _joy1
				sta _joy1
				lda byte1_4017_1
				ora _joy2
				sta _joy2
		@skipHori:
		
	@skipDetectMultitap:
	
	rts
.endproc