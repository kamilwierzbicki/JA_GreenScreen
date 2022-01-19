.model flat, stdcall

PUBLIC rmGreenScreen
.code
;*************************************************************************
;			rmGreenScreen
;*************************************************************************
;	source - array with image
;	dest - destination array which will be used to save modified image
;	len - length of source array
;	minDif - min Difference between green and other colors to make it 
;	transparent
;*************************************************************************
;	Modifies source image and saves it to destination array.
;	If current pixel has green byte with higher value than 
;	max(blue, red) + minimal difference, function changes
;	alpha byte to 0.
;	Modified image has transparent green pixels.
;*************************************************************************
	rmGreenScreen PROC source:DWORD, dest:DWORD, len:DWORD, minDif:BYTE
	push ebx
	sub esp, 4
	mov dword ptr[ebp-8], 0

rmGreen_loop:
	push source
	push dword ptr [ebp-8]
	push 02h
	call insrtBytesToXMM ; Copying green from pixels to xmm register
	MOVDQA xmm7, xmm0
	push source
	push dword ptr [ebp-8]
	push 01h
	call insrtBytesToXMM ; Copying red from pixels to xmm register
	movdqa xmm6, xmm0 ; xmm6 will be register that contains only red color
	push source
	push dword ptr[ebp-8]
	push 03h
	call insrtBytesToXMM ; Copying blue from pixels to xmm register
	movdqa xmm5, xmm0

	PMAXUB xmm5,xmm6

	mov eax, 80h ; This part of code is responsible for adding 0x80 to every byte of colors
	pinsrb xmm0, eax, 0 ; It is needed because comparing unsigned bytes is not supported by avx and sse
	; (Comparing unsigned bytes came in avx-512)
	VPBROADCASTB xmm4, xmm0
	paddb xmm5, xmm4
	paddb xmm7, xmm4
	movdqa xmm6, xmm7
	PCMPGTB xmm7, xmm5 ;now in xmm7 is mask which informs us about which green bytes were greater than red and blue bytes

	;Filling xmm4 register with minimal difference given in argument
	xor eax, eax
	mov al, minDif
	pinsrb xmm0, eax, 0
	VPBROADCASTB xmm4, xmm0

	pand xmm5, xmm7 ; now in xmm5 are only red or blue bytes which are lower than green bytes
	pand xmm6, xmm7 ; In xmm6 are only green bytes which are greater than other colors

	psubsb xmm6, xmm5

	PCMPGTB xmm4, xmm6


	; Comparing red and blue bytes with green bytes. If green byte is greater than both of them
	;there will be 0x00 on its position, otherwise it will be 0xff
	;PCMPGTB xmm5, xmm7
;	PMPGTB xmm6, xmm7
;	por xmm5, xmm6
	;PCMPEQB   xmm1, xmm1
	;PANDN     xmm6, xmm1 ; Bitwise not xmm5
	movdqa xmm0, xmm4
	mov eax, len
	sub eax, [ebp-8]
	push eax
	push [ebp-8]
	push dest
	call unpckAlphaFromXmm
	add dword ptr[ebp-8], 16
	mov eax, [ebp-8]
	;mov ebx, 4
	;mul ebx
	cmp eax, len
	jng rmGreen_loop


	add esp, 4
	pop ebx
	ret
	rmGreenScreen ENDP
;*************************************************************************
;						END OF rmGreenScreen
;*************************************************************************

;*************************************************************************
;							insrtBytesToXMM
;*************************************************************************
;	colorOffs - offset of color to copy to xmm0 register
;	pixelOffs - offset of pixel to copy
;	source - array with pixels to copy
;*************************************************************************
;	Copies specified color mm0 register.
;*************************************************************************
	insrtBytesToXMM PROC colorOffs:DWORD, pixelOffs:DWORD, source:DWORD
	push ebx
	mov ebx, source
	sub esp, 8
	mov edx, 60
	xor eax,eax
	mov eax, colorOffs
	add edx, eax
	push edx
	mov eax, pixelOffs
	mov edx, 4
	mul edx
	pop edx
	add ebx,eax
	xmm0_LOOP:
	PSLLDQ xmm0, 1
	pinsrb xmm0, BYTE PTR[ebx+edx], 0
	
	sub edx, 4
	cmp edx, 0
	jns xmm0_LOOP

	pop ebx
	ret
	pop ebx
	ret
	insrtBytesToXMM ENDP
;*************************************************************************
;						END OF insrtBytesToMMX
;*************************************************************************

;*************************************************************************
;						unpckAlphaFromXmm
;*************************************************************************
;	dest - Array with pixels to save new alpha channel
;	offs - Offset
;*************************************************************************
;	Unpacks alpha channel from xmm0 register and saves it to dest array
;*************************************************************************
	unpckAlphaFromXmm PROC dest:DWORD, offs:DWORD, len:DWORD
	push ebx
	sub esp, 4
	mov DWORD PTR[ebp-8], 0 ; local variable to store current index of reading byte in xmm register
	mov eax, len
	cmp eax, 16
	jng unpck_start
	mov len, 16 ; If len is longer than 16 replace it with 16
unpck_start:
	mov ecx, offs ; ecx will store byte offset in array
	mov eax, ecx  ; byte offset is 4 times higher than pixel offset 
	mov ebx, 4	  ; because in one pixel there are 4 channels 
	mul ebx
	mov ecx, eax
	mov edx, dest
unpck_loop:
	xor eax, eax
	pextrb eax,xmm0, 0
	mov BYTE PTR[edx + ecx], al ; reading byte from xmm register 
	PSRLDQ xmm0, 1	; Shifting xmm register to right by 8 bits (1 byte)  
	mov eax, [ebp-8]
	add eax, 1
	mov [ebp-8], eax ; Incrementing current index
	add ecx, 4
	mov eax, [ebp-8]
	sub eax, len
	jnz unpck_loop ; if current index is lower than len jump back
	ret
	pop ebx
	unpckAlphaFromXmm ENDP

;*************************************************************************
;					END OF unpckAlphaFromXmm
;*************************************************************************

END   