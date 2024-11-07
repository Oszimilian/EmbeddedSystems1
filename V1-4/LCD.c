/*----------------------------------------------------------------------------
  Funktionen f�r LC-Display
	
	06/2020
 *----------------------------------------------------------------------------*/
#include "I2C.h"

#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_tim.h>


#define LCD_ADR	0x27		// I2C-Adresse des LCDisplays

#define RS 0x01					// Bit f�r Daten (gesetzt f�r Daten, gel�cht f�r Kommando)
#define RW 0x02					// Read/Write (gesetzt f�r Read)
#define E 0x04					// Enabe (fallende Flanke �bernimmt Daten/Kommandos)
#define BL 0x08					// Hintergrundbeleuchtung an/aus

void DelayMsNew(int ms)
// wartet ms Millisekunden unter Verwendung von Timer6
{
	if (ms==0)																							// wenn Wartezeit=0, dann sofort zurueck
		return;
	
	// Timer initialisieren
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);			// Tim6 Takt einschalten
		
	LL_TIM_SetPrescaler(TIM6,SystemCoreClock/1000-1);				// Prescaler f�r 1kHz = 1ms
	LL_TIM_SetAutoReload(TIM6,ms);													// Wartezeit in Auto-Reload-Register
	LL_TIM_SetCounter(TIM6,0);															// Z�hler auf 0 zur�cksetzen
	LL_TIM_EnableCounter(TIM6);															// Z�hler starten
		
	while (LL_TIM_GetCounter(TIM6)<ms)
		;
	
	LL_TIM_DisableCounter(TIM6);
}

void LCDSendCmd (char cmd)
// sendet Kommando an Display
{
  unsigned char data_u, data_l;
	unsigned char data_t[4];
	
	data_u = (cmd&0xf0);			// obere 4 Bit des Kommandos
	data_l = ((cmd<<4)&0xf0);	// untere 4 Bit des Kommandos
	
	// Kommando senden
	data_t[0] = data_u|BL|E;  //en=1, rs=0
	data_t[1] = data_u|BL;  	//en=0, rs=0
	data_t[2] = data_l|BL|E;  //en=1, rs=0
	data_t[3] = data_l|BL;  	//en=0, rs=0
	I2CWrite(data_t,4,LCD_ADR);
}

void LCDSendData (char data)
// sendet Daten an Display
{
	char data_u, data_l;
	unsigned char data_t[4];
	
	if (data == '^') data = 0x00;
	
	data_u = (data&0xf0);			// obere 4 Bit des Kommandos
	data_l = ((data<<4)&0xf0);	// untere 4 Bit des Kommandos
	
	data_t[0] = data_u|BL|E|RS;  //en=1, rs=0
	data_t[1] = data_u|BL|RS;  	//en=0, rs=0
	data_t[2] = data_l|BL|E|RS;  //en=1, rs=0
	data_t[3] = data_l|BL|RS;  	//en=0, rs=0
	I2CWrite(data_t,4,LCD_ADR);
	
}

void LCDClear(void)
{
	LCDSendCmd(0x01);
	DelayMsNew(2);
}

void LCDSendString (char *str)
{
	while (*str) LCDSendData (*str++);
}

void LCDDefineDegree(void)
{
	uint8_t data[8] = {0x1C, 0x14, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00};
	for (int i = 0; i < 8; i++) {
			uint8_t tmp = (1<<6) | (0x07 & i);
			LCDSendCmd(tmp);
			LCDSendData(data[i]);
	}
	
	LCDSendCmd(0x80);
	DelayMsNew(2);
}
