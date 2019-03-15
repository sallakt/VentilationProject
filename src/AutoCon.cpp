/*
 * AutoCon.cpp
 *
 *  Created on: Mar 11, 2019
 *      Author: vovan
 */

#include "AutoCon.h"
#include "ITM_conv.h"

/**
 * Set the frequency of the controller.
 * can be used beween a change con manual to automatic mode
 * to use the current frequency and not start from 0
 * @param int f - Frequency
 */
void AutoCon::setFreq(int f){
	freq = f;
}

/**
 * @return TRUE when the the controller reached the goal the first time
 */
bool AutoCon::goalReached(){
	return reached;
}

/**
 * Reset the reachedGoal flag,
 * should be used when the user sets a new pressure
 */
void AutoCon::newGoal(){
	reached = false;
}

AutoCon::AutoCon()
{}

/**
 * Automatic speed controller.
 * Calculates a frequency based on the current frequency and the difference
 * between the userinput an the current pressure.
 * @param I2CMaster* I2Cread - pressure sensor to get the current pressure
 * @param float input - the pressure the controller should try to reach
 * @return uint32_t - calculated frequency
 */
uint32_t AutoCon::adjust(I2CMaster* I2Cread, float input)
{
	uint8_t received[3];
	I2Cread->ReadValueI2CM(received, 3);

	int16_t pres = ((int16_t)received[0] << 8) | received[1];

	//READING VALUES FOR TESTING
	p.print("\n PSA: ");
	p.print(pres/240*0.95f);
	p.print(" / Goal: ");
	p.print(input);
	p.print("\n");

	// calculate difference
	float diff = input - (pres/240*0.95f);
	if (diff < 0)
	{
		// increase the speed based on the difference of the input to the psa
		// 80 = correction value for the smaller steps
		freq = (int)freq+(20*diff-80);
	}

	else if(diff > 0)
	{
		// decrease the speed based on the difference of the input to the psa
		// 80 = correction value for the smaller steps
		freq = (int)freq+(20*diff+80);
	}else{
		// flag to deactivate the error when the goal is reached
		reached = true;
	}


	// limit the frequency to the max possible
	if(freq > maxFreq){freq = (int) maxFreq;}
	p.print(freq);p.print("\n");
	return freq;
}





