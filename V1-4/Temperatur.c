/*----------------------------------------------------------------------------
  Funktionen für Temperatursensor	MCP 9808
	
	06/2020
 *----------------------------------------------------------------------------*/

#include "I2C.h"

#define TEMP_ADR    0x18  // Slave Adresse Temperaturfühler
#define TEMP_RES    0x08  // Resolution Register
#define TEMP_TR     0x05  // Temperature Register

uint16_t TempRead()
// liest Temperatur aus und gibt sie in 1/16 Grad zurück
{
	int ret = 0;
	
	uint8_t tmp_data[2];
	tmp_data[0] = TEMP_TR;
	I2CWrite(tmp_data, 1, TEMP_ADR);
	I2CRead(tmp_data, 2, TEMP_ADR);
	
	return (tmp_data[0] << 8 | tmp_data[1]);
}

int TempSetRes(int resolution)
// Auflösung des Temperatursensors setzen (Modi 0...3 erlaubt)
{ 
	if (resolution > 3 || resolution < 0) return -1;
	
	uint8_t tmp_data[2];
	tmp_data[0] = TEMP_RES;
	tmp_data[1] = (uint8_t)resolution;
	I2CWrite(tmp_data, 2, TEMP_ADR);
	
	return 0;
}
