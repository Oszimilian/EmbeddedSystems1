/*----------------------------------------------------------------------------
  Funktionen für LEDs mit LL-Library
	
	01/2022
 *----------------------------------------------------------------------------*/

#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_bus.h>

void LEDInit(void)
// initialisiert LED-Ausgänge GPIOB4...7
{
	// GPIOB3...7 initialisieren
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);			// GPIOB Takt einschalten
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0,LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_1,LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_2,LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_3,LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_4,LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_5,LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6,LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_7,LL_GPIO_MODE_OUTPUT);
}

void LEDSet(int nr)
// LED mit nr (0...7) einschalten
{
	if (nr==0)
		LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_0);

	if (nr==1)
		LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_1);

	if (nr==2)
		LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_2);

	if (nr==3)
		LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_3);
	
	if (nr==4)
		LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_4);

	if (nr==5)
		LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_5);

	if (nr==6)
		LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_6);

	if (nr==7)
		LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_7);
}

void LEDReset(int nr)
// LED mit nr (0...7) ausschalten
{
	if (nr==0)
		LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_0);

	if (nr==1)
		LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_1);

	if (nr==2)
		LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_2);

	if (nr==3)
		LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_3);
	
	if (nr==4)
		LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_4);

	if (nr==5)
		LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_5);

	if (nr==6)
		LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_6);

	if (nr==7)
		LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_7);
}
