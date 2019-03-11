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



class AutoCon
{
public:
	Autcon::AutoCon(I2CMaster* i2c);
	virtual ~AutoCon(){};
	void AutoCon::adjust(I2CMaster* I2Cread);
private:
	uint8_t baseVal;


}

#endif /* AUTONCON_H_ */
