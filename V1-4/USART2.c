/*----------------------------------------------------------------------------
  Funktionen für USART mit LL-Library
	
	06/2020
 *----------------------------------------------------------------------------*/

#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_usart.h>

char USART2InputBuffer[64];											// Eingabepuffer für Befehle
int USART2CmdFlag=0;														// Flag für anliegendes Kommando

void USART2Init(void)
// Initialisierung der USART2-Schnittstelle
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);		// GPIOA Takt einschalten

	// PA2 initialisieren
	LL_GPIO_SetPinSpeed(GPIOA,LL_GPIO_PIN_2,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_2,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_0_7(GPIOA,LL_GPIO_PIN_2,LL_GPIO_AF_7);

	// PA3 initialisieren
	LL_GPIO_SetPinSpeed(GPIOA,LL_GPIO_PIN_3,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_3,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_0_7(GPIOA,LL_GPIO_PIN_3,LL_GPIO_AF_7);

	
	// USART2 initialisieren
	// ...

	NVIC_EnableIRQ(USART2_IRQn);														// NVIC aktivieren
}

void USART2WriteChar(char c)
// schreibt Zeichen c an USART2
{

	// ...

}

void USART2WriteString(char *str)
// schreibt String str an USART2
{

	// ...

}

/*----------------------------------------------------------------------------
  Interrupt-Handler USART2
 *----------------------------------------------------------------------------*/
void USART2_IRQHandler (void)
{

	// ...

}
