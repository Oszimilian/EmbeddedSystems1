int main(void) {
  volatile unsigned int* rcc_ptr = 0;
	volatile unsigned int* systick_ptr = 0;
	volatile unsigned int* led_ptr = 0;
	
	rcc_ptr = (unsigned int*)(0x40023800 + 0x30);
	*rcc_ptr |= 0x01;
	
	systick_ptr = (unsigned int*)(0xE000E010 + 0x04);
	*systick_ptr = 16000000;
	
	led_ptr = (unsigned int*)(0x40020000);
	*led_ptr |= 0x400;
	
	systick_ptr = (unsigned int*)(0xE000E010);
	*systick_ptr |= 0x01 | 0x04;
	
	led_ptr = (unsigned int*)(0x40020000 + 0x14);
	
	while(1) {
		
		if( (*systick_ptr & (1<<16)) ) {
			*led_ptr = *led_ptr ^ (1 << 5);
		}
		
	}
	
	
}
