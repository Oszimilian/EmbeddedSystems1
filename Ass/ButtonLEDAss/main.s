	THUMB
	AREA	myCode, CODE
	EXPORT 	Start
		
Start
	; init rcc for port a and c
	ldr r0, =0x40023800
	ldr r1, =0x05
	str r1, [r0, #0x30]
	
	; set pa5 to output
	ldr r0, =0x40020000
	ldr r1, [r0]
	ldr r2, =0x400
	orr r1, r2
	str r1, [r0]
	
	; set pc13 to input
	ldr r0, =0x40020800
	ldr r1, [r0]
	ldr r2, =0xF3FFFFFF
	and r1, r2
	str r1, [r0]
	
LOOP
	ldr r0, =0x40020800
	ldr r1, [r0, #0x10]
	ldr r2, =0x2000
	and r1, r2
	cmp r1, r2
	beq LEDOFF
	b LEDON
	
	
	
	
LEDON
	ldr r0, =0x40020000
	ldr r1, [r0]
	ldr r2, =0x20
	orr r1, r2
	str r1, [r0, #0x14]
	b LOOP
	
LEDOFF
	ldr r0, =0x40020000
	ldr r1, [r0]
	ldr r2, =0xFFFFFFDF
	and r1, r2
	str r1, [r0, #0x14]
	b LOOP
	
	
	END
	