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
#include <string>
#include <atomic>

class Menu {
public:
			Menu(LiquidCrystal *lcd, DigitalIoPin *b1, DigitalIoPin *b2, DigitalIoPin *b3, void (*fPtr)(int));
	uint8_t getSpeed();
	void 	setSpeed(uint8_t speed); 	// only for Automaticmode
	void 	setPsa(uint8_t psa);		// only for Manualmode
	uint8_t getPsa();
	void 	checkInputs();		// Check the state of all Buttons
	bool 	getMode();
	void 	updateDisplay();
	bool 	hasNewValue();		// returns true if the Speed has changed
	bool 	hasNewGoal();
	void 	clear();
	void 	error(std::string msg);
	virtual ~Menu();
private:
	void 	changeMode();
	bool	manualMode = true;
	uint8_t speed = 0;
	uint8_t	psa = 0;
	bool 	changed = false;
	bool 	goal = false;
	LiquidCrystal *lcd;
	DigitalIoPin *b1;	// MODE
	DigitalIoPin *b2;	// UP
	DigitalIoPin *b3;	// DOWN
	void (*Sleep)(int);
	volatile int lpb1;
	volatile int lpb2;
	volatile int lpb3;
};


#endif /* MENU_H_ */
