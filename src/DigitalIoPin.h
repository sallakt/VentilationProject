/*
 * DigitalIoPin.h
 *
 *  Created on: 22 Jan 2019
 *      Author: oli
 */

#ifndef DIGITALIOPIN_H_
#define DIGITALIOPIN_H_

class DigitalIoPin {
public:
	DigitalIoPin(int port, int pin, bool input = true, bool pullup = true, bool invert = false);
	virtual ~DigitalIoPin();
	bool read(void);
	void write(bool value);

private:
	int pin;
	int port;
	bool input;
};

#endif /* DIGITALIOPIN_H_ */
