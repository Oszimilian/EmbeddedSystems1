#include <stdint.h>

#define I2Cx I2C1					// Auswahl des I2C-Bus
#define CLOCKSPEED 100000	// Taktrate des I2C-Bus

void I2CInit(uint8_t OwnAddress);
void I2CWrite(const uint8_t* buf, uint32_t nbyte , uint8_t SlaveAddress);
void I2CRead(uint8_t *buf, uint32_t nbyte , uint8_t SlaveAddress);
