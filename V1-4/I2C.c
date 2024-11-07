/*----------------------------------------------------------------------------
  Funktionen für I2C mit LL-Library
	
	Nutzung der I2C-Schnittstelle des Nucleo 
	
	06/2020
 *----------------------------------------------------------------------------*/

//#include "i2c.h"
#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_i2c.h>
#include "I2C.h"

void I2CInit(uint8_t OwnAddress)
// initialisiert I2C
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);			// GPIOB Takt einschalten

	// GPIO PB8 initialisieren
	LL_GPIO_SetPinSpeed(GPIOB,LL_GPIO_PIN_8,LL_GPIO_SPEED_FREQ_MEDIUM);
	LL_GPIO_SetPinMode(GPIOB,LL_GPIO_PIN_8,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_8_15(GPIOB,LL_GPIO_PIN_8,LL_GPIO_AF_4);
	
	// GPIO PB9 initialisieren
	LL_GPIO_SetPinSpeed(GPIOB,LL_GPIO_PIN_9,LL_GPIO_SPEED_FREQ_MEDIUM);
	LL_GPIO_SetPinMode(GPIOB,LL_GPIO_PIN_9,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinOutputType(GPIOB,LL_GPIO_PIN_9,LL_GPIO_OUTPUT_OPENDRAIN);
	LL_GPIO_SetAFPin_8_15(GPIOB,LL_GPIO_PIN_9,LL_GPIO_AF_4);

	// I2C initialisiseren
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);				// I2C Takt einschalten
	LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_I2C1);				// I2C Reset auslösen
	LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_I2C1);			// I2C Reset freigeben

	LL_I2C_ConfigSpeed(I2Cx,SystemCoreClock,CLOCKSPEED,LL_I2C_DUTYCYCLE_2);
	LL_I2C_SetOwnAddress1(I2Cx,OwnAddress<<1,LL_I2C_OWNADDRESS1_7BIT);
	LL_I2C_SetMode(I2Cx,LL_I2C_MODE_I2C);
	LL_I2C_AcknowledgeNextData(I2Cx,LL_I2C_ACK);
	LL_I2C_Enable(I2Cx);																			// I2C starten
}

void I2CWrite(const uint8_t* buf, uint32_t nbyte , uint8_t SlaveAddress)
// schreibt von Inhalt von buf insgesamt nbyte Zeichen an Slave SlaveAddress
{
	if (!nbyte)																	// keine Daten zu senden, Ende
		return;
	
	SlaveAddress=SlaveAddress<<1;								// Adresse muss um ein Bit verschoben werden

	while(LL_I2C_IsActiveFlag_BUSY(I2Cx))				// warten, bis Bus frei
		;
	LL_I2C_DisableBitPOS(I2Cx);									// ACK kontrolliert aktuelles Byte 
	LL_I2C_GenerateStartCondition(I2Cx);				// Startbedingung generieren
	while (!LL_I2C_IsActiveFlag_SB(I2Cx))				// wartet auf Startbedingung
		;
	LL_I2C_TransmitData8(I2Cx, SlaveAddress);		// Slave-Adresse senden
	while (!LL_I2C_IsActiveFlag_ADDR(I2Cx))			// wartet auf Antwort Slave
		;
	LL_I2C_ClearFlag_ADDR(I2Cx);
	while (!LL_I2C_IsActiveFlag_TXE(I2Cx))			// wartet bis Sendepuffer leer
		;
	LL_I2C_TransmitData8(I2Cx, *buf++);					// sendet erstes Byte
	
	while (--nbyte) {
		while (!LL_I2C_IsActiveFlag_BTF(I2Cx))		// wartet, bis Byte gesendet
			;
		LL_I2C_TransmitData8(I2Cx, *buf++);				// sendet nächstes Byte
	}
	
	LL_I2C_GenerateStopCondition(I2Cx);					// generiert Stopbedingung
}

void I2CRead(uint8_t *buf, uint32_t nbyte , uint8_t SlaveAddress)
// liest insgesamt nbyte von Slave SlaveAddress nach buf
{
	if (!nbyte)																	// keine Daten zu senden, Ende
		return;

	SlaveAddress=SlaveAddress<<1;								// Adresse muss um ein Bit verschoben werden
	
	while(LL_I2C_IsActiveFlag_BUSY(I2Cx))				// warten, bis Bus frei
		;
	LL_I2C_DisableBitPOS(I2Cx);									// ACK kontrolliert aktuelles Byte 
	LL_I2C_AcknowledgeNextData(I2Cx,LL_I2C_ACK);
	LL_I2C_GenerateStartCondition(I2Cx);				// Startbedingung generieren
	while (!LL_I2C_IsActiveFlag_SB(I2Cx))				// wartet auf Startbedingung
		;
	LL_I2C_TransmitData8(I2Cx, SlaveAddress|1);	// Slave-Adresse für lesen senden
	while (!LL_I2C_IsActiveFlag_ADDR(I2Cx))			// wartet auf Antwort Slave
		;

	if (nbyte==1){															// wenn nur 1 Byte zu senden
		LL_I2C_AcknowledgeNextData(I2Cx,LL_I2C_NACK);
		LL_I2C_ClearFlag_ADDR(I2Cx);
		LL_I2C_GenerateStopCondition(I2Cx);				// generiert Stopbedingung
	} else if (nbyte==2) {											// wenn 2 Byte zu senden
		LL_I2C_AcknowledgeNextData(I2Cx,LL_I2C_NACK);
		LL_I2C_EnableBitPOS(I2Cx);								// ACK kontrolliert nächstes Byte 
		LL_I2C_ClearFlag_ADDR(I2Cx);
	} else {																		// wenn mehr als 2 Byte zu senden
		LL_I2C_AcknowledgeNextData(I2Cx,LL_I2C_ACK);
		LL_I2C_ClearFlag_ADDR(I2Cx);
	}

	while (nbyte>0)	{															// solange noch Bytes zu übertragen sind
		if (nbyte<=3) {
			if (nbyte==1) {
				while (!LL_I2C_IsActiveFlag_RXNE(I2Cx))	// warten, bis Sendepuffer leer
					;
				*buf++ = LL_I2C_ReceiveData8(I2Cx);			// Byte lesen
				nbyte--;
			} else if (nbyte==2) {
				while (!LL_I2C_IsActiveFlag_BTF(I2Cx))	// warten, bis Byte übertragen
					;
				LL_I2C_GenerateStopCondition(I2Cx);			// generiert Stopbedingung
				*buf++ = LL_I2C_ReceiveData8(I2Cx);			// Byte lesen
				nbyte--;
				*buf++ = LL_I2C_ReceiveData8(I2Cx);			// Byte lesen
				nbyte--;
			} else {
				while (!LL_I2C_IsActiveFlag_BTF(I2Cx))	// warten, bis Byte übertragen
					;
				LL_I2C_AcknowledgeNextData(I2Cx,LL_I2C_NACK);
				*buf++ = LL_I2C_ReceiveData8(I2Cx);			// Byte lesen
				nbyte--;
				while (!LL_I2C_IsActiveFlag_BTF(I2Cx))	// warten, bis Byte übertragen
					;
				LL_I2C_GenerateStopCondition(I2Cx);			// generiert Stopbedingung
				*buf++ = LL_I2C_ReceiveData8(I2Cx);			// Byte lesen
				nbyte--;
				*buf++ = LL_I2C_ReceiveData8(I2Cx);			// Byte lesen
				nbyte--;
			}
		} else {
			while (!LL_I2C_IsActiveFlag_RXNE(I2Cx))	// warten, bis Sendepuffer leer
				;
			*buf++ = LL_I2C_ReceiveData8(I2Cx);			// Byte lesen
			nbyte--;
			if (LL_I2C_IsActiveFlag_BTF(I2Cx)) {
				*buf++ = LL_I2C_ReceiveData8(I2Cx);			// Byte lesen
				nbyte--;
			}
		}
	}
}
