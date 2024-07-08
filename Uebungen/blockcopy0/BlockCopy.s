; Programm "Kopieren eines Speicherbereichs"
; Kopieren vom Flash-Speicher in RAM-Bereich 
; rel. Speicherorte für Quelle und Ziel (Basis: buffer)


; den Assemblerdialekt deklarieren
		THUMB
		AREA	Programm, CODE, ALIGN=2
			
		EXPORT	Start
Start
;*****************************************************************

		; pointer setzen
		ldr		r12, = buffer		; r12 zeigt auf buffer
		ldr		r10, = TEXT			; r10 zeigt auf Text
		ldr     r11, = TEXT_END		; r11 zeigt auf das Textende
		
Loop    cmp r11, r10
		blt done
		ldr r0, [r10],#3
		str r0, [r12], #3
		
		b Loop

		;..... eigenes Programm

done	b		done				; Sprung auf sich selbst, Endlosschleife als Programmende
;********************************************************************
TEXT	 DCB		"Hochschule Augsburg55 "	; zu kopierendes Feld
TEXT_END DCB 0

;********************************************************************
		AREA	Daten, DATA	; R/W Memory
buffer	SPACE	100 		; 100 Byte Platz		

		END
