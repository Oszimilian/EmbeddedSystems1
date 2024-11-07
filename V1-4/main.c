/*----------------------------------------------------------------------------
	MCT-Praktikumsversuche V1...4 Nucleo mit LL-Library
		
	08/2022
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_tim.h>
#include "main.h"
#include "LED.h"
#include "ADC.h"
#include "v1_adc.h"
#include "DAC.h"
#include "math.h"
#include "I2C.h"
#include "LCD.h"
#include "LCDInit.h"
#include "Temperatur.h"


void USART2Init(void);
void USART2WriteChar(char c);
void USART2WriteString(char *str);

void TIM3InitPWM(void);
void TIM3Servo (int pos);
void TIM3RGB(int *rgb);
void setCCR(int channel, int value);


extern char USART2InputBuffer[];
extern int USART2CmdFlag;



extern int StepDir;  		// Richtung des Schrittmotors
extern int AktPos;			// aktuelle Position des Schrittmotors
extern int ZielPos;			// Zielposition des Schrittmotors
extern int StepMode;		// Modus des Schrittmotors
extern int Ticks;



#define PI 3.14159

#define __IO volatile							// Datentyp fl?chtig
#define __IM     volatile const      /*! Defines 'read only' structure member permissions */
#define __IOM    volatile            /*! Defines 'read / write' structure member permissions */
	
#define PERIPH_BASE           0x40000000UL /*!< Peripheral base address in the alias region                                */
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000UL)	// Basisadresse Peripherie
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800UL)	// RCC-Basisadresse
#define RCC                 	((RCC_TypeDef *) RCC_BASE)		// RCC-Register
#define GPIOB_BASE            (AHB1PERIPH_BASE + 0x0400UL)
#define GPIOC_BASE            (AHB1PERIPH_BASE + 0x0800UL)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)
#define SCS_BASE            (0xE000E000UL)                            /*!< System Control Space Base Address */
#define SysTick_BASE        (SCS_BASE +  0x0010UL)                    /*!< SysTick Base Address */


typedef struct
// Struktur f?r SysTick-Register
{
  __IOM uint32_t CTRL;		// Kontrollregister
  __IOM uint32_t LOAD;		// Register Startwert
  __IOM uint32_t VAL;			// aktueller Wert
  __IM  uint32_t CALIB;		// Kalibration
} SysTick_TypeDef;

volatile unsigned int counter2 = 0;
volatile unsigned int steps[8][4] = {{1,1,0,0},{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,1,1},{0,0,0,1},{1,0,0,1},{1,0,0,0}};
volatile unsigned int steps_pos = 0;
volatile unsigned int dir = 2;
volatile unsigned int act_pos = 0;
volatile unsigned int des_pos = 0;
volatile unsigned char mode = 'o';
volatile unsigned int flag = 0;
volatile unsigned int temp_count = 0;


void SysTick_Handler() {
	
	
	
	for (int i = 0; i < 4; i++) {
		if (dutycyles[i] > counter) {
			LEDSet(i + 4);
		} else {
			LEDReset(i + 4);
		}
	}
	
	if (counter2++ == 45
		){
		
		for (int i = 0; i < 4; i++) {
			if (steps[steps_pos][i]) {
				LEDSet(i);
			} else {
				LEDReset(i);
			}
		} 
		
		int diff = des_pos - act_pos;
		if (mode == 'm') {
			if(diff > 0) {
				dir = 0;
			} else if (diff < 0) {
				dir = 1;
			} else {
				dir  = 2;
			}
		}

			
		counter2 = 0;
		if (dir == 0) {
			steps_pos = (steps_pos == 7)?0:steps_pos+1;
			if (mode == 'm')
				act_pos++;
		} 
		else if(dir == 1) {
			steps_pos = (steps_pos == 0)?7:steps_pos-1;
			if (mode == 'm')
				act_pos--;
		}
	}
	
	counter = (counter == 100)? 0 : counter + 1;
	
	temp_count++;
	
}

void setDir(int input_dir) {
	dir = input_dir;
}

void SysTickTime(int ms) {
	volatile SysTick_TypeDef* sys_tick_ptr;
	sys_tick_ptr = (volatile SysTick_TypeDef*)(SysTick_BASE);
	
	sys_tick_ptr->CTRL |= 0x04;
	sys_tick_ptr->LOAD = 400 * (4 / ++ms);
}

void SysTickEnable() {
	volatile SysTick_TypeDef* sys_tick_ptr;
	sys_tick_ptr = (volatile SysTick_TypeDef*)(SysTick_BASE);
	
	sys_tick_ptr->CTRL |= 0x03;
}

void DelayMs(int ms)
// wartet ms Millisekunden unter Verwendung von Timer6
{
	if (ms==0)																							// wenn Wartezeit=0, dann sofort zurueck
		return;
	
	// Timer initialisieren
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);			// Tim6 Takt einschalten
		
	LL_TIM_SetPrescaler(TIM6,SystemCoreClock/1000-1);				// Prescaler für 1kHz = 1ms
	LL_TIM_SetAutoReload(TIM6,ms);													// Wartezeit in Auto-Reload-Register
	LL_TIM_SetCounter(TIM6,0);															// Zähler auf 0 zurücksetzen
	LL_TIM_EnableCounter(TIM6);															// Zähler starten
		
	while (LL_TIM_GetCounter(TIM6)<ms)
		;
	
	LL_TIM_DisableCounter(TIM6);
}


int sin_vals[32] = {2000,2372,2729,3056,3340,3569,3734,3828,3847,3790,3660,3462,3205,2897,2553,2187,1813,1447,1103,795,538,340,210,153,172,266,431,660,944,1271,1628,2000};




void sinus_berechnung()
{
	for(int i = 0; i < 32; i++)
	{
		double angle = (2 * PI * i) / 31;
		double sin_val = sin(angle);
		sin_vals[i] = (int)(2048 + 1850 * sin_val);
	}
}


void ExecCmd(	int* delay, 
							int* amp, 
							int* pos, 
							int* rgb, 
							volatile unsigned int* dir, 
							volatile unsigned char* step_mode, 
							volatile unsigned int* dest_pos_step,
							float* temp,
							int* resolution,
							unsigned int *clock_temp_en) {
	if (USART2CmdFlag == 1) {
		
		char c1 = 'z';
		char c2 = 'z';
		char c3 = 'z';
		int i1 = -1;
		int i2 = 0;
		int i3 = 0;
		
		
		int ret = -1;
		ret = sscanf(USART2InputBuffer, "%c%d%c%d%c%d", &c1, &i1, &c2, &i2, &c3, &i3);
		
		char err_str[50];
		
		switch(c1) {
			case 'd':
									*delay = i1;
									if (c2 == 'a') {
										*amp = i1;
										sprintf(err_str, "Delay: %d Amps: %d\r\n", *delay, *amp);
									} else {
										sprintf(err_str, "INPUT ERROR\r\n");
									}
									break;
					
			case 's': 
									*pos = i1; 
									sprintf(err_str, "Pos: %d\r\n", *pos);
									break;
			case 'r': 
									rgb[0] = i1;
									if (c2 == 'g' && c3 == 'b') {
										rgb[1] = i2;
										rgb[2] = i3;
										sprintf(err_str, "R: %d G: %d B: %d\r\n", rgb[0], rgb[1], rgb[2]);
									} else {
										sprintf(err_str, "INPUT ERROR\r\n");
									}
									break;
									
			case 'o': 
									*step_mode = c1;
									sscanf(USART2InputBuffer, "%c%c", &c1, &c2);
									if (c2 == '+') {
										*dir = 0;
									}else if(c2 == '-') {
										*dir = 1;
									} else {
										*dir = 2;
									}
									sprintf(err_str, "StepMode: %d Dir: %d\r\n", *step_mode, *dir);
									break;
			case 'm':
									*step_mode = c1;
									*dest_pos_step = i1;
									sprintf(err_str, "StepMode: %d DestPos: %d\r\n", *step_mode, *dest_pos_step);
									break;
			
			case 't':
								  sscanf(USART2InputBuffer, "%c%c%i", &c1, &c2, &i1);
									if(c2 == 'r') {
										switch(*resolution) {
											case 0: sprintf(err_str, "Temp: %.1f\r\n", *temp); break;
											case 1: sprintf(err_str, "Temp: %.2f\r\n", *temp); break;
											case 2: sprintf(err_str, "Temp: %.3f\r\n", *temp); break;
											case 3: sprintf(err_str, "Temp: %.4f\r\n", *temp); break;
											default: break;
										}
										
									}
									else if(c2 == 's'){
										int ret = TempSetRes(i1);
										*resolution = i1;
										if (ret == -1) {
											sprintf(err_str, "Cant set Res! \r\n");
										}
									}
									else{
										sprintf(err_str, "Input Error\r\n");
									}
									break;
				
				case 'l':
									sscanf(USART2InputBuffer, "%c%c%x", &c1, &c2, &i1);
									if(c1 == 'l') {
										if(c2 == 'c') {
											if (i1 == -1) goto ERROR;
											LCDSendCmd(i1);
										}
										else if(c2 == 'd') {
											if (i1 == -1) goto ERROR;
											LCDSendData(i1);
										}
										else{
											ERROR:
											sprintf(err_str, "Input Error\r\n");
										}
									}
									
				case 'v':
						if (i1) {
							sprintf(err_str, "Temp Clock EN\r\n");
							*step_mode = 'm';
							*clock_temp_en = 1;
						} else {
							sprintf(err_str, "Temp Clock DIS\r\n");
							*step_mode = '\0';
							*clock_temp_en = 0;
						}
						break;
				
			default: ret = -1; break;
		}
		
		if (ret <= 0) {
			char* err_str = "Input Error\r\n";
		}
		
		USART2WriteString(err_str);
		USART2CmdFlag = 0;
	}
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

int isPosChanged(int pos) {
	static int last_pos = 0;
	if (last_pos != pos) {
		last_pos = pos;
		return 1;
	} else {
		return 0;
	}
}

typedef union {
	struct {
		unsigned int tmp_lower : 8;
		unsigned int tmp_upper : 4;
		unsigned int sign : 1;
		unsigned int lower : 1;
		unsigned int upper : 1;
		unsigned int crit : 1;
	} bits;
	uint16_t all;
} tmp_field;

float calcTemp(tmp_field* temp_field_ptr) {
	int lower = temp_field_ptr->bits.tmp_lower;
	int upper = temp_field_ptr->bits.tmp_upper;
	float tmp = (float)(upper * 16) + (float)(lower) / 16.0;
	if (temp_field_ptr->bits.sign) {
		tmp = 256 - tmp;
		tmp *= -1;
	}
	return tmp;
}

void printTempToDisplay(float temp, int resolution) {
	if (temp_count / 5000) {
		LCDClear();
		char c[50];
		switch(resolution) {
			case 0: sprintf(c, "Temp: %.1f^C", temp); break;
			case 1: sprintf(c, "Temp: %.2f^C", temp); break;
			case 2: sprintf(c, "Temp: %.3f^C", temp); break;
			case 3: sprintf(c, "Temp: %.4f^C", temp); break;
			default: break;
		}
		LCDSendString(c);
		temp_count = 0;
	}
} 

void visTemp(float temp, volatile unsigned int* pos, volatile unsigned int* clock_temp_en) {
	if (temp >= 20.0f && temp <= 30.0f && *clock_temp_en == 1) {
		*pos = 180 / 10 * (temp - 20);
	}
}

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void)           

{ 	
	
	//------- Init ----------
	LEDInit();									// LEDs initialisieren
	// ...										// ADC Kanal 0 initialisieren
	DACInit();
	sinus_berechnung();
	USART2Init();
	
	ADCInit(0);
	
	I2CInit(8);
	
	LCDInit();
	
	int delay = 1;
	int amp = 1;
	int pos = 50;
	int rgb[3] = {0, 0, 0};
	tmp_field temp;
	float real_temp;
	int resolution = 3;
	unsigned int clock_temp_en = 0;
	
	SysTickTime(1);
	SysTickEnable();
	
	TIM3InitPWM();

	initPortC();
	
	
	LCDClear();
	LCDDefineDegree();
	
	char err_str[50];
	while (1) 
	{
		
		temp.all = TempRead();
		real_temp = calcTemp(&temp);
		printTempToDisplay(real_temp, resolution);
		visTemp(real_temp, &des_pos, &clock_temp_en);

		int adc_val = ADCRead();
		calc_led(adc_val);
		
		ExecCmd(&delay, &amp, &pos, rgb, &dir, &mode, &des_pos, &real_temp, &resolution, &clock_temp_en);
		if (isPosChanged(pos)) {
			TIM3Servo(pos);
			DelayMs(1000);
			TIM3Servo(-1);
		}
		
		TIM3RGB(rgb);
		
		
		for (int i = 0; i < 31; i++) {
			DACWrite(sin_vals[i]/amp);
			DelayMs(delay);
		}
		
	}
}
