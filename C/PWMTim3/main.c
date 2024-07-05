
int main(void) {
	//rcc PORTA
	volatile unsigned int* p = 0;
	p = (unsigned int*)(0x40023800 + 0x30);
	*p |= 0x01;
	
	//rcc TIM3
	p = (unsigned int*)(0x40023800 + 0x40);
	*p |= 0x02;
	
	//tim3 prescaler
	p = (unsigned int*)(0x40000400 + 0x28);
	*p |= 159;
	
	//tim3 autoreload
	p = (unsigned int*)(0x40000400 + 0x2c);
	*p |= 2000;
	
	
	
	
}