/*
 * AutoCon.cpp
 *
 *  Created on: Mar 11, 2019
 *      Author: vovan
 */

#include "AutonCon.h"

AutoCon::AutoCon(I2CMaster* i2c)
{
	uint8_t received[3];
	i2c->ReadValueI2CM(received, 3);
	baseVal = received[1];
}

AutoCon::adjust(I2CMaster* I2Cread)
{
	uint8_t received[3];
	I2Cread->ReadValueI2CM(received, 3);
}


