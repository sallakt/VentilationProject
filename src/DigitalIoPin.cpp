/*
 * DigitalIoPin.cpp
 *
 *  Created on: 22 Jan 2019
 *      Author: oli
 */

#include "DigitalIoPin.h"
#include "chip.h"


DigitalIoPin::DigitalIoPin(int port, int pin, bool input, bool pullup, bool invert) {
	this->port = port;
	this->pin = pin;
	this->input = input;

	uint8_t mod;
	if(pullup && invert && input) {
		mod = (IOCON_DIGMODE_EN | IOCON_MODE_PULLUP | IOCON_INV_EN);
	}else if(pullup && input){
		mod = (IOCON_DIGMODE_EN | IOCON_MODE_PULLUP);
	}else if(invert && input){
		mod = (IOCON_DIGMODE_EN | IOCON_INV_EN);
	}else{
		mod = (IOCON_DIGMODE_EN | IOCON_MODE_INACT );
	}

	Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, mod);
	if(input){
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
	}else{
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, port, pin);
		Chip_GPIO_SetPinState(LPC_GPIO, port, pin, true);
	}

}

bool DigitalIoPin::read(){
	return Chip_GPIO_GetPinState(LPC_GPIO, port, pin);
}

void DigitalIoPin::write(bool on){
	if(!input){
		Chip_GPIO_SetPinState(LPC_GPIO, port, pin, on);
	}
}



DigitalIoPin::~DigitalIoPin() {

}

