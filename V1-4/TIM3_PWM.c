/*----------------------------------------------------------------------------
  Funktionen für PWM (Servo und RGB-LED)
	
	06/2020
 *----------------------------------------------------------------------------*/

#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_tim.h>

void TIM3InitPWM(void)
// Initialisierung des Timer 3 für PWM (Servo + RGB-LED)
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);			// GPIOA Takt einschalten
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);			// GPIOC Takt einschalten

	// PA6 (Servo) initialisieren
	LL_GPIO_SetPinSpeed(GPIOA,LL_GPIO_PIN_6,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_6,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_0_7(GPIOA,LL_GPIO_PIN_6,LL_GPIO_AF_2);

	// PA7 (LED rot) initialisieren
	LL_GPIO_SetPinSpeed(GPIOA,LL_GPIO_PIN_7,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_7,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_0_7(GPIOA,LL_GPIO_PIN_7,LL_GPIO_AF_2);

	// PC8 (LED grün) initialisieren
	LL_GPIO_SetPinSpeed(GPIOC,LL_GPIO_PIN_8,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_8,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_8_15(GPIOC,LL_GPIO_PIN_8,LL_GPIO_AF_2);

	// PC9 (LED blau) initialisieren
	LL_GPIO_SetPinSpeed(GPIOC,LL_GPIO_PIN_9,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_9,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_8_15(GPIOC,LL_GPIO_PIN_9,LL_GPIO_AF_2);

	// Timer initialisieren
	// RCC TIM3
	volatile unsigned int *ptr = (unsigned int*)(0x40023800 + 0x40);
	*ptr |= (1<<1);
	
	// PRESACALER
	ptr = (unsigned int*)(0x40000400 + 0x28);
	*ptr = 159;
	
	// AUTO RELOAD
	ptr = (unsigned int*)(0x40000400 + 0x2c);
	*ptr = 2000;
	
	// OUTPUT COMPARE MODE => PWM MODE 1
	ptr = (unsigned int*)(0x40000400 + 0x18);
	*ptr |= ( ((1<<6) | (1<<5)) | ((1<<13) | (1<<14)) );
	ptr = (unsigned int*)(0x40000400 + 0x1C);
	*ptr |= ( ((1<<6) | (1<<5)) | ((1<<13) | (1<<14)) );
	
	// CCER
	ptr = (unsigned int*)(0x40000400 + 0x20);
	*ptr |= ((1<<0) | (1<<4) | (1<<8) | (1<<12));
	
	// TIM3 ENABLE
	ptr = (unsigned int*)(0x40000400 + 0x00);
	*ptr |= (1<<0);
	
	// Timer starten
	// ...
}

void setCCR(int channel, int value) {
		if (value < 0 || value > 2000) return;
		
		volatile unsigned int *ptr = NULL;
		switch(channel) {
			case 0: ptr = (unsigned int*)(0x40000400 + 0x34); break;
			case 1: ptr = (unsigned int*)(0x40000400 + 0x38); break;
			case 2: ptr = (unsigned int*)(0x40000400 + 0x3c); break;
			case 3: ptr = (unsigned int*)(0x40000400 + 0x40); break;
			default: break;
		}
		if(ptr)
			*ptr = value;
}

void TIM3Servo (int pos)
// Servo auf Position fahren (0...100)
{
	
  if ((pos <0 || pos >100)) {	
		setCCR(0, 0);
		return;
	}
	
	setCCR(0, 100+pos);
}

void TIM3RGB(int *rgb) {
// RGB-LED-Helligkeit setzen (0...255)
	for (int i = 0; i < 3; i++) {
		if (rgb[i] >= 0 && rgb[i] <= 254) {
			setCCR(i+1, (int)(2000-(rgb[i] * 2000 / 254)));
		}
	}
}
