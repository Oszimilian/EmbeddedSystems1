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


			ldr   r0, DEZ_ZAHL
			ldr   r10, =_string 
			mov   r1, #10 		; Basis 10
			mov   r5, #0x30 	; ASCII -Offset

			;..........

stop		b		.  ; Ende of program
;************************************************************			

;************************************************************			
; Place for code segment based initialized constants
			ALIGN
			LTORG

DEZ_ZAHL 	DCD		1234	; 

			

;************************************************************			
            AREA    Daten, DATA
string		SPACE    10 ;
_string 


			END



