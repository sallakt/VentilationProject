/*
 * I2CMaster.h
 *
 *  Created on: 29 Jan 2019
 *      Author: oli
 */

#include <cstdio>
#include <stdarg.h>

#ifndef I2CMASTER_H_
#define I2CMASTER_H_
/* I2C clock is set to 1.8MHz */
#define I2C_CLK_DIVIDER         (40)
/* 100KHz I2C bit-rate */
#define I2C_BITRATE         (100000)
/* Standard I2C mode */
#define I2C_MODE    (0)
/* 7-bit I2C addresses of Temperature Sensor */
#define I2C_TEMP_ADDR_7BIT  (0x48) // OK for TD74A0

namespace I2C_Master {

class I2C_Master {
	public:
		I2C_Master();
		virtual ~I2C_Master();
		uint8_t ReadValueI2CM(int size);
	private:
		void Init_I2C_PinMux(void);
		void SetupXferRecAndExecute(uint8_t devAddr,
										   uint8_t *txBuffPtr,
										   uint16_t txSize,
										   uint8_t *rxBuffPtr,
										   uint16_t rxSize);

};

} /* namespace I2C_Master */

#endif /* I2CMASTER_H_ */
