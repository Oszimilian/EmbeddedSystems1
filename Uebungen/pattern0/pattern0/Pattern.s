; pattern
; *********************************************************************


; Assembler basic configuration  (Stack Alignment, THUMB, UAL)
			PRESERVE8
			THUMB
			AREA	Programm, CODE

			EXPORT	Start

;************************************************************			
Start		; Start of program

LEN			equ		27
STARTADR	equ 	0x100
; Hier wird zunächst der  Bereich byteweise mit dem Muster 0xAA belegt
			ldr 	r10,=buffer
			ldr     r11, =STARTADR	;  Startadresse des Bereichs berechnen
			add		r10,r10,r11
			ldr		r0, =LEN  			; Länge als Zähler in r0
			mov     r9, #1				; 1 wird im sub-Befehl benötigt
			ldrb 	r1, PATTERN_AA		;  Muster im r1 speichern
loop		strb	r1, [r10],#1	 	; Muster in den Speicher kopieren
			sub 	r0,r0,r9		   	; Zähler dekrementieren, 
			;sub 	r0,r0,#1		   	; Zähler dekrementieren, Variante mit Konstante 										; verwendet
			cmp		r0,#0				; Test auf Scheifenende
			bne		loop
;***********************************
; Hier erfolgt die eigentliche Aufgabe, nämlich den Speicherbereich mit dem 4Byte-Muster zu beschreiben.
; Es wird dabei das Muster 0xAA überschrieben


			; ....
			; Hier steht das Belegen mit PATTERN_55
			; Es soll mit 32 Bit Muster gearbeitet werden
			; benoetigt wird der Befehl udiv r1,r2,r3; (r1=r2/r3)
			; ....

	
stop		b		.  ; Ende of program
;************************************************************			

;************************************************************			
; Place for code segment based initialized constants
			ALIGN
			LTORG

PATTERN_55	DCD		0x55555555	; Testmuster
PATTERN_AA	DCB		0xAA		; Testmuster
			
;************************************************************			
            AREA    Daten,DATA
buffer		SPACE	  0x1000				; 

			END



