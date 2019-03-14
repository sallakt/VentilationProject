/*
 * AutonCon.h
 *
 *  Created on: Mar 11, 2019
 *      Author: vovan
 */

#ifndef AUTOCON_H_
#define AUTOCON_H_


#include "I2C.h"
#include "I2CMaster.h"
#include "ModbusMaster.h"
#include "ModbusRegister.h"
#include "ITM_conv.h"
#include "ITM_write.h"
#include "Menu.h"

#define minFreq 2000.00000
#define maxFreq 20000.00000
#define maxPress 120.00000

//UNFINISHED
class AutoCon
{
	friend bool setFrequency(ModbusMaster& node, uint16_t freq, void (*Sleep)(int));
public:
	AutoCon(I2CMaster* i2c);
	virtual ~AutoCon(){};
	void adjust(I2CMaster* I2Cread, float input, ModbusMaster& mbWrite, void (*Sleep)(int));

private:
	uint8_t baseVal;
	ITM_conv p; //FOR TESTING


};

#endif /* AUTOCON_H_ */
