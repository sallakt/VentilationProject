/*
 * AutoCon.cpp
 *
 *  Created on: Mar 11, 2019
 *      Author: vovan
 */

#include "AutoCon.h"
#include "ITM_conv.h"

void AutoCon::setFreq(int f){
	freq = f;
}

bool AutoCon::goalReached(){
	return reached;
}

void AutoCon::newGoal(){
	reached = false;
}

bool setFrequency(ModbusMaster& node, uint16_t freq, void (*Sleep)(int))
{
	int result;
	int ctr;
	bool atSetpoint;
	const int delay = 500;
	ModbusRegister Frequency(&node, 1); // reference 1
	ModbusRegister StatusWord(&node, 3);
	Frequency = freq; // set motor frequency

	// wait until we reach set point or timeout occurs
	ctr = 0;
	atSetpoint = false;
	do {
		Sleep(delay);
		// read status word
		result = StatusWord;
		// check if we are at setpoint
		if (result >= 0 && (result & 0x0100)) atSetpoint = true;
		ctr++;
	} while(ctr < 20 && !atSetpoint);

	return atSetpoint;
}

AutoCon::AutoCon(I2CMaster* i2c)//UNFINISHED
{
	uint8_t received[3];
	i2c->ReadValueI2CM(received, 3);
}

uint32_t AutoCon::adjust(I2CMaster* I2Cread, float input, ModbusMaster& mbWrite, void (*Sleep)(int))//UNFINISHED
{
	uint8_t received[3];
	I2Cread->ReadValueI2CM(received, 3);

	int16_t pres = ((int16_t)received[0] << 8) | received[1];
	//READING VALUES FOR TESTING
	p.print("\n Values: ");
		p.print(pres/240*0.95f);
		p.print("\n");
	float diff = input - (pres/240*0.95f);
//	float error = 20000 - freq;
	if (diff < 0)
	{
		//freq = (int)freq+(20*diff-error*0.01)/*(diff/maxPress)*(maxFreq)*/;
		freq = (int)freq+(20*diff-80)/*(diff/maxPress)*(maxFreq)*/;
	}

	else if(diff > 0)
	{
		//freq = (int)freq+(20*diff+error*0.01)/*(diff/maxPress)*(maxFreq)*/;
		freq = (int)freq+(20*diff+80)/*(diff/maxPress)*(maxFreq)*/;
	}else{
		reached = true;
	}



//	p.print(freq);p.print("\n");

	if(freq > maxFreq){freq = (int) maxFreq;}
	p.print(freq);p.print("\n");
	setFrequency(mbWrite, freq, Sleep);
	return freq;
}





