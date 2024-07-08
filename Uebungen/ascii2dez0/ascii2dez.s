;Dezimal to Ascii
; Die Dezimalzahl soll als ASCII-Zeichen in string stehen
; *********************************************************************


; Assembler basic configuration  (Stack Alignment, THUMB, UAL)
			PRESERVE8
			THUMB
			AREA	MyCode, CODE

			EXPORT	Start

;************************************************************			
Start		; Start of program



			ldr   r10, =dez_str;
		
			mov   r1, #10 		; Basis 10
			mov   r5, #0x30 	; ASCII -Offset
            ;....
			;....
			
stop		b		.  ; Ende of program
;************************************************************			

;************************************************************			
; Place for code segment based initialized constants
			ALIGN
			LTORG

dez_str     DCB  "73125",0;
			

;************************************************************			
            AREA    Daten, DATA
 
Zahl		SPACE 4;

			END



