/*
 * Menu.h
 *
 *  Created on: 1 Mar 2019
 *      Author: oli
 */

#ifndef MENU_H_
#define MENU_H_
#include "LiquidCrystal.h"
#include "DigitalIoPin.h"

class Menu {
public:
			Menu(LiquidCrystal *lcd, DigitalIoPin *b1, DigitalIoPin *b2, DigitalIoPin *b3, void (*fPtr)(int));
	uint8_t getSpeed();
	void 	setSpeed(uint8_t speed); // only for Automatic
	void 	setPsa(uint8_t psa);
	uint8_t getPsa();
	void 	checkInputs();	// Check the state of all Buttons
	bool 	getMode();
	void 	updateDisplay();
	bool 	hasNewValue();		// returns true if the Speed has changed
	void 	clear();
	virtual ~Menu();
private:
	void 	changeMode();
	bool	manualMode = true;
	uint8_t speed = 0;
	uint8_t	psa = 0;
	bool 	changed = false;
	LiquidCrystal *lcd;
	DigitalIoPin *b1;	// MODE
	DigitalIoPin *b2;	// UP
	DigitalIoPin *b3;	// DOWN
	void (*Sleep)(int);
};


#endif /* MENU_H_ */
