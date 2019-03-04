/*
 * I2CMaster.cpp
 *
 *  Created on: 29 Jan 2019
 *      Author: oli
 */

#include "I2CMaster.h"
#include "chip.h"

namespace I2C_Master {


/* I2CM transfer record */
static I2CM_XFER_T  i2cmXferRec;


I2C_Master::I2C_Master() {

	/* Setup I2C  */
	Init_I2C_PinMux();

	/* Enable I2C clock and reset I2C peripheral - the boot ROM does not
	   do this */
	Chip_I2C_Init(LPC_I2C0);

	/* Setup clock rate for I2C */
	Chip_I2C_SetClockDiv(LPC_I2C0, I2C_CLK_DIVIDER);

	/* Setup I2CM transfer rate */
	Chip_I2CM_SetBusSpeed(LPC_I2C0, I2C_BITRATE);

	/* Enable Master Mode */
	Chip_I2CM_Enable(LPC_I2C0);

	NVIC_DisableIRQ(I2C0_IRQn);

}

void I2C_Master::SetupXferRecAndExecute(uint8_t devAddr,
								   uint8_t *txBuffPtr,
								   uint16_t txSize,
								   uint8_t *rxBuffPtr,
								   uint16_t rxSize)
{
	/* Setup I2C transfer record */
	i2cmXferRec.slaveAddr = devAddr;
	i2cmXferRec.status = 0;
	i2cmXferRec.txSz = txSize;
	i2cmXferRec.rxSz = rxSize;
	i2cmXferRec.txBuff = txBuffPtr;
	i2cmXferRec.rxBuff = rxBuffPtr;

	Chip_I2CM_XferBlocking(LPC_I2C0, &i2cmXferRec);
}

void I2C_Master::Init_I2C_PinMux(void)
{
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 22, IOCON_DIGMODE_EN | I2C_MODE);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 23, IOCON_DIGMODE_EN | I2C_MODE);
	Chip_SWM_EnableFixedPin(SWM_FIXED_I2C0_SCL);
	Chip_SWM_EnableFixedPin(SWM_FIXED_I2C0_SDA);
}

uint8_t I2C_Master::ReadValueI2CM(int size)
{
	uint8_t values[size];
	uint8_t lm75TempRegisterAddress = 0xF1;

	/* Read LM75 temperature sensor */
	SetupXferRecAndExecute(

		/* The LM75 I2C bus address */
		I2C_TEMP_ADDR_7BIT,

		/* Transmit one byte, the LM75 temp register address */
		&lm75TempRegisterAddress, 1,

		/* Receive back two bytes, the contents of the temperature register */
		values, size);

	/* Test for valid operation */
	if (i2cmXferRec.status == I2CM_STATUS_OK) {
		/* Output temperature.
		return ("Temperature read over I2C is %d.%d degrees C.\r\n",
			(int) temperature[0], (temperature[1] & 0x80) ? 5 : 0 );
			*/
		return values[1];
	}

	return values[1];

}

I2C_Master::~I2C_Master() {
	// TODO Auto-generated destructor stub
}

} /* namespace I2C_Master */
