; searchmin
; die kleinste Zahl der Zahlenreihe wird in min abgespeichert

; den Assemblerdialekt deklarieren
		THUMB

		AREA	Hauptprogramm, CODE, ALIGN=2
	
		EXPORT	Start
;*******************************************************************
Start	ldr		r12, = min		; r12 ist pointer auf min
		ldr		r11, = zahlen	; r11 ist pointer auf zahlen
		ldr		r10, = _zahlen	; r10 ist pointer auf Feldende 
		ldr r0, [r11], #4
		str r0, [r12]
		
Loop    cmp r11, r10
		bge done
		ldr r0, [r11], #4
		ldr r1, [r12]
		cmp r0, r1
		bgt Loop
		str r0, [r12]
		b Loop

;	..... eigenes Programm ....

done	b done					; Stop
;*******************************************************************
zahlen	DCD		7,9,5,3,2,8,10,5	; zu durchsuchendes Feld
_zahlen


;*******************************************************************
		AREA	Daten, DATA	; R/W Memory
min		DCD		0	; Variable für min

		END
