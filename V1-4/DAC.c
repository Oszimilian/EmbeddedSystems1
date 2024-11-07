/*----------------------------------------------------------------------------
  Funktionen für Digital-Analog-Konverter mit LL-Library
	
	06/2020
 *----------------------------------------------------------------------------*/

#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_dac.h>

void DACInit(void)
// Initialisierung des DAC-Konverters
{
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_DAC1);			// DAC Takt einschalten

	LL_DAC_Enable(DAC,LL_DAC_CHANNEL_1);										// DAC einschalten
}

void DACWrite(int analog)
// Ausgabe des Analogwertes
{
	LL_DAC_ConvertData12RightAligned(DAC,LL_DAC_CHANNEL_1,analog);	 				// DAC-Wert ausgeben
}
