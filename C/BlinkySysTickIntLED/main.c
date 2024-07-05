#include<stm32f446xx.h>

void SysTick_Handler(void);

int main(void) {
	volatile unsigned int* rcc_ptr = 0;
	volatile unsigned int* led_ptr = 0;
	volatile unsigned int* systick_ptr = 0;

	
	rcc_ptr = (unsigned int*)(0x40023800 + 0x30);
	*rcc_ptr |= 0x01;
	
	led_ptr = (unsigned int*)(0x40020000);
	*led_ptr |= 0x400;
	
	systick_ptr = (unsigned int*)(0xE000E010 + 0x04);
	*systick_ptr = 16000000;
	systick_ptr = (unsigned int*)(0xE000E010);
	*systick_ptr |= (0x04 | 0x02 | 0x01);
	
	while(1) {}
}

void SysTick_Handler(void) {
	volatile unsigned int* led_ptr = 0;
	led_ptr = (unsigned int*)(0x40020000 + 0x14);
	*led_ptr ^= (1<<5);
}
