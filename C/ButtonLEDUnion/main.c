
typedef union {
	struct{
		unsigned int enable : 1;
		unsigned int tick_int : 1;
		unsigned int clk_src : 1;
		unsigned int unused_1 : 13;
		unsigned int countflag : 1;
		unsigned int unused_2 : 15;
	}bits;
	unsigned int all;
} STK_CRL;


int main(void) {
	volatile STK_CRL* ptr = (volatile STK_CRL*)(0xE000E010);
	ptr->bits.enable = 1;
	
	
	
	unsigned  int reg = ptr->all;
}
