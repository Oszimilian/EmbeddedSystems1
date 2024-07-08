	area tpl, code, READONLY
	
	EXPORT __main
		
__main
	; enable port a 
	ldr r0, =0x40023800
	ldr r1, =0x01
	ldr r2, [r0]
	orr r2, r1
	str r2, [r0, #0x30]
	
	; set pa5 to output
	ldr r0, =0x40020000
	ldr r1, [r0]
	ldr r2, =0x400
	orr r1, r2
	str r1, [r0]
	
	ldr r0, =0xE000E010
	ldr r1, [r0, #0x04]
	ldr r2, =16000000
	orr r1, r2
	str r1, [r0, #0x04]
	
	ldr r1, [r0]
	ldr r2, =0x05
	orr r1, r2
	str r1, [r0]
	
LOOP
	ldr r0, =0xE000E010
	ldr r1, [r0]
	ldr r2, =(1<<16)
	and r1, r2
	cmp r1, r2
	beq TOGGLE
	b LOOP
	
	
TOGGLE
	ldr r0, =0x40020000
	ldr r1, [r0, #0x14]
	ldr r2, =(1<<5)
	eor r1, r2
	str r1, [r0, #0x14]
	b LOOP
	
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