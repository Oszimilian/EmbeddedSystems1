
volatile int counter = 0;
volatile int dutycyles[4];


void set_dutycycles(int* dutys) {
	for (int i = 0; i < 4; i++) {
		dutycyles[i] = dutys[i];
	}
}

void calc_led(int adc_val) {
	int most_sig_bits = ((adc_val & 0xC00) >> 10);
	int leas_sig_bits = (adc_val & 0x3FF);
	
	int pwm_pos = 0;
	int tmp[4] = {0,0,0,0};
	switch(most_sig_bits) {
		case 0x00: 	pwm_pos = 0; break;
		case 0x01: 	tmp[0] = 100; 
								pwm_pos = 1; break;
		case 0x02: 	tmp[0] = 100; tmp[1] = 100; 
								pwm_pos = 2; break;
		case 0x03: 	tmp[0] = 100; tmp[1] = 100; tmp[2] = 100;
								pwm_pos = 3; break;
		
		default: break;
	}
	
	
	tmp[pwm_pos] = (leas_sig_bits >> 2);
	
	set_dutycycles(tmp);
}

