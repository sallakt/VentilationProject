/*
 * AutonCon.h
 *
 *  Created on: Mar 11, 2019
 *      Author: vovan
 */

#ifndef AUTONCON_H_
#define AUTONCON_H_
#include "I2C.h"
#include "I2CMaster.h"
#include "ModbusMaster.h"
#include "ModbusRegister.h"
#include "ITM_conv.h"
#include "ITM_write.h"


//UNFINISHED
class AutoCon
{
	friend bool setFrequency(ModbusMaster& node, uint16_t freq, void (*Sleep)(int));
public:
	AutoCon(I2CMaster* i2c);
	virtual ~AutoCon(){};
	void adjust(I2CMaster* I2Cread, ModbusMaster* mbWrite);

private:
	uint8_t baseVal;
	ITM_conv p; //FOR TESTING


};

#endif /* AUTONCON_H_ */
