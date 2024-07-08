
void initPortA(void);
void initTIM3(void);
void initTIM3CH2(void);
void setCCR(unsigned int ccr_val);
void initPortC(void);
char isBlueButtonPressed(void);

void initPortA(void) {
	//rcc PORTA and PORTC
	volatile unsigned int* p = 0;
	p = (unsigned int*)(0x40023800 + 0x30);
	*p |= ((1<<0)|(1<<2));
	
	//MODER PORTA
	p = (unsigned int*)(0x40020000);
	*p &= ~(unsigned int)((1<<14)|(1<<15));
	*p |= (1<<15);
	
	//AFLR
	p = (unsigned int*)(0x40020000 + 0x20);
	*p &= ~(0xF0000000);
	*p |= 0x20000000;
}

void initTIM3(void) {
	volatile unsigned int* p = 0;
	//rcc TIM3
	p = (unsigned int*)(0x40023800 + 0x40);
	*p |= (1<<1);
	
	//tim3 prescaler
	p = (unsigned int*)(0x40000400 + 0x28);
	*p = 159;
	
	//tim3 autoreload
	p = (unsigned int*)(0x40000400 + 0x2c);
	*p = 2000;
	

}

void initTIM3CH2(void) {
	volatile unsigned int* p = 0;
	
	//ccmr
	p = (unsigned int*)(0x40000400 + 0x18);
	*p &= ~(unsigned int)((1<<12));
	*p |= ((1<<13)|(1<<14));
	
	//ccer
	p = (unsigned int*)(0x40000400 + 0x20);
	*p |= ((1<<4));
	
	//enable
	p = (unsigned int*)(0x40000400);
	*p |= (1<<0);
	
	setCCR(1500);
}

void setCCR(unsigned int ccr_val) {
	volatile unsigned int* p = 0;
	p = (unsigned int*)(0x40000400 + 0x38);
	*p = ccr_val;
}

void initPortC(void) {
	volatile unsigned int* p = 0;
	p = (unsigned int*)(0x40020800);
	*p &= ~(unsigned int)((1<<27)|(1<<26));
}

char isBlueButtonPressed(void) {
	volatile unsigned int* p = 0;
	p = (unsigned int*)(0x40020800 + 0x10);
	
	return (*p & (1<<13))?1:0;
}

int main(void) {
	initPortA();
	initTIM3();
	initTIM3CH2();
	initPortC();
	
	setCCR(100);
	while(1) {
		if(isBlueButtonPressed()) {
			setCCR(100);
		}else{
			setCCR(1500);
		}
	}
	
}
