/*----------------------------------------------------------------------------
  Funktionen für Analog-Digital-Konverter miz LL-Library
	
	04/2022
 *----------------------------------------------------------------------------*/

#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_adc.h>

void ADCInit(int ADCChannel)
// initialisiert ADC
// ADCChannel: Auswahl ADC-Kanal, nur 0 oder 1 zulässig!
{
	if (ADCChannel!=0 && ADCChannel!=1)														// Abbruch, wenn Kanal nicht 0 oder 1
		return;

	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);					// Takt GPIOA einschalten
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1); 					// Takt ADC1 einschalten
	
	// Initialisierung ADC1
	if (ADCChannel==0)
		LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_0,LL_GPIO_MODE_ANALOG);	// GPIO PA0 auf Analogmodus
	else
		LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_1,LL_GPIO_MODE_ANALOG);	// GPIO PA1 auf Analogmodus
	
	LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(__ADC1__),LL_ADC_CLOCK_SYNC_PCLK_DIV2); 			//Prescaler von ADC auf 2
	LL_ADC_SetSequencersScanMode(ADC1,LL_ADC_SEQ_SCAN_DISABLE);		// Sequentielles Sacnnen abschalten
	if (ADCChannel==0)
		LL_ADC_REG_SetSequencerRanks(ADC1,LL_ADC_REG_RANK_1,LL_ADC_CHANNEL_0); // Kanal 0 in der Reiehnfolge auf Rang 1 setzen
	else
		LL_ADC_REG_SetSequencerRanks(ADC1,LL_ADC_REG_RANK_1,LL_ADC_CHANNEL_1); // Kanal 1 in der Reiehnfolge auf Rang 1 setzen
	LL_ADC_REG_SetContinuousMode(ADC1,LL_ADC_REG_CONV_SINGLE);		// Einzelwandlungsmodus
	LL_ADC_REG_SetTriggerSource(ADC1,LL_ADC_REG_TRIG_SOFTWARE);		// Softwaregetriggert
	if (ADCChannel==0)
		LL_ADC_SetChannelSamplingTime(ADC1,LL_ADC_CHANNEL_0,LL_ADC_SAMPLINGTIME_3CYCLES);	// Sampling Time (ggf. verschiedenen Werte und Rauschen bewerten)
	else
		LL_ADC_SetChannelSamplingTime(ADC1,LL_ADC_CHANNEL_1,LL_ADC_SAMPLINGTIME_3CYCLES);	// Sampling Time (ggf. verschiedenen Werte und Rauschen bewerten)
	LL_ADC_SetResolution(ADC1,LL_ADC_RESOLUTION_12B);							// Auflösung 12 Bit
	LL_ADC_SetDataAlignment(ADC1,LL_ADC_DATA_ALIGN_RIGHT);				// Daten rechtsbündig

	LL_ADC_Enable(ADC1);																					// ADC einschalten
}

int ADCRead(void)
// liest Wert vom ADC
{
	int adc_val = 0;
	LL_ADC_REG_StartConversionSWStart(ADC1);
	while(!LL_ADC_IsActiveFlag_EOCS(ADC1));
	adc_val = LL_ADC_REG_ReadConversionData12(ADC1);
	return adc_val;
}

