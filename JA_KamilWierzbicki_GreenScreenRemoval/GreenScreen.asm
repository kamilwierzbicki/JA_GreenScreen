.model  flat

_TEXT  SEGMENT

removeGreenScreenAssembly PROC
	push ebp
	mov DWORD PTR[ebp - 4], ecx
	mov dword ptr [ebp-8],edx 
	pop ebp
removeGreenScreenAssembly ENDP


_TEXT  ENDS

END   