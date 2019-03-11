/*
 * Menu.cpp
 *
 *  Created on: 1 Mar 2019
 */

#include "Menu.h"

/*
 * @param LiquidCrystal *lcd - Pointer to the display controller
 * @param DigitalIoPin *b1 - Pointer to the fist menu button - MODE
 * @param DigitalIoPin *b2 - Pointer to the secound menu button - UP
 * @param DigitalIoPin *b3 - Pointer to the third menu button - DOWN
 * @param void (*fPtr)(int) - Pointer to the Sleep function of the main
 */
Menu::Menu(LiquidCrystal *lcd, DigitalIoPin *b1, DigitalIoPin *b2, DigitalIoPin *b3, void (*fPtr)(int)) {
	this->lcd = lcd;
	this->b1 = b1;
	this->b2 = b2;
	this->b3 = b3;
	this->Sleep = fPtr;
	//manualMode = false;
}

Menu::~Menu() {
	clear();
}

void Menu::clear(){
	lcd->clear();
}


/*
 * Retruns the Speed value
 * Used in Manualmode
 */
uint8_t Menu::getSpeed(){
	return speed;
}

/*
 * Set the Speed value to print it on the display
 * Used in Automaticmode to display the calculated value
 * @param uint8_t s - calculated speed mode from the Automaticmode
 */
void Menu::setSpeed(uint8_t speed){
	if(!manualMode && (speed <= 100 || speed >= 0)){
		this->speed = speed;
	}
}

/*
 * Retruns the displayed PSA value
 */
uint8_t Menu::getPsa(){
	return psa;
}

/*
 * Set the PSA value to print it on the display
 * @param uint8_t psa - calculated psa value from the sensor
 */
void Menu::setPsa(uint8_t psa){
	if(psa > 0){
		if(this->psa != psa){
			changed = true;
		}
		this->psa = psa;
	}

}

/*
 * Check all Buttons if there is some new userinput
 * eventually move this to the main and add two new Methods (incSpeed / decSpeed)
 */
void Menu::checkInputs(){

	if(b1->read()){
		Sleep(3);
		if(lpb1 % 800){
			if(manualMode){
				if(speed<100){
					speed++;
					changed = true;
				}
			}else{
				if(psa<120){
					psa++;
					changed = true;
				}
			}
		}
		lpb1++;
		if(!b1->read()){// if Manual increase speed
			lpb1 = 0;
			if(manualMode){
				if(speed<100){
					speed++;
					changed = true;
				}
			}else{
				if(psa<120){
					psa++;
					changed = true;
				}
			}

		}
	}
	if(b3->read()){
		Sleep(3);
		if(lpb3 % 800){
			if(manualMode){
				if(speed>0){
					speed--;
					changed = true;
				}
			}else{
				if(psa>0){
					psa--;
					changed = true;
				}
			}
		}
		lpb3++;
		if(!b3->read()){// if Manual increase speed
			if(manualMode){
				if(speed>0){
					speed--;
					changed = true;
				}
			}else{
				if(psa>0){
					psa--;
					changed = true;
				}
			}
			lpb3=0;
		}
	}


	if(b2->read()){ // Change Mode
		Sleep(3);
		if(!b2->read()){
			changeMode();
			changed = true;
		}
	}

}

/*
 * Update the data on the Display
 */
void Menu::updateDisplay(){
	lcd->setCursor(0, 0);
	lcd->print("Fan Speed: ");
	lcd->setCursor(11, 0);
	lcd->print(std::to_string(speed));

	// Display Mode
	lcd->setCursor(15, 0);
	lcd->print(manualMode ? "M" : "A");

	lcd->setCursor(0, 1);
	lcd->print("Pressure: ");
	lcd->setCursor(11, 1);
	lcd->print(std::to_string(psa));
}

void Menu::error(std::string msg){
	lcd->clear();
	lcd->setCursor(0, 0);
	lcd->print("Error: ");
	lcd->setCursor(0, 1);
	lcd->print(msg);
	Sleep(1000);
}

/*
 * Returns true if the user changed in manualmode the speed;
 */
bool Menu::hasNewValue(){
	bool val = changed;
	changed = false;
	return val;
}

/*
 *  Returns the current mode
 *  TRUE = Manualmode
 *  FALSE = Automaticmode
 */
bool Menu::getMode(){
	return manualMode;
}

/*
 * Changed the mode between Manual and Automatic
 */
void Menu::changeMode(){
	manualMode = !manualMode;
}
