int main(void) {
	volatile unsigned int* rcc_ptr = 0;
	volatile unsigned int* led_ptr = 0;
	volatile unsigned int* button_ptr = 0;
	
	rcc_ptr = (unsigned int*)(0x40023800 + 0x30);
	*rcc_ptr = 0x05;
	
	led_ptr = (unsigned int*)(0x40020000);
	*led_ptr |= 0x400;
	
	button_ptr = (unsigned int*)(0x40020800);
	*button_ptr &= 0xF3FFFFFF;
	
	button_ptr = (unsigned int*)(0x40020800 + 0x10);
	led_ptr = (unsigned int*)(0x40020000 + 0x14);
	
	while(1) {
		
		if (!(*button_ptr & 0x2000)) {
		  *led_ptr |= 0x20;
		} else {
			*led_ptr &= 0xFFDF;
		}
		
	}
}
