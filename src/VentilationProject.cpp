/*
 * @brief Blinky example using timers and sysTick
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" withlcd any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software withlcd notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use withlcd further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, withlcd fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */
#include "chip.h"
#include "board.h"
#include <atomic>
#include <cctype>
#include <string>
#include "DigitalIoPin.h"
#include "Menu.h"
#include "LiquidCrystal.h"
#include "I2CMaster.h"
#include "ITM_conv.h"
#include "ModbusMaster.h"
#include "ModbusRegister.h"
#include "LpcUart.h"

static volatile std::atomic_int counter;
static volatile std::atomic_int sensorCounter;
static volatile uint32_t systicks;

#define TICKRATE_HZ1 (1000)
#define TASK (1)

ITM_conv *p;
Menu *m;
I2CMaster *i;


#ifdef __cplusplus
extern "C" {
	#endif
	void SysTick_Handler(void)
	{
		systicks++;
		sensorCounter--;
		if(counter > 0)
		{
			counter--;

		}else{

		}

	}
	#ifdef __cplusplus
}
#endif

void Sleep(int ms)
{
	counter = ms;

	while(counter > 0){
		__WFI();
	}
}

uint32_t millis() {
	return systicks;
}

bool setFrequency(ModbusMaster& node, uint16_t freq)
{
	int result;
	int ctr;
	bool atSetpoint;
	const int delay = 500;

	ModbusRegister Frequency(&node, 1); // reference 1
	ModbusRegister StatusWord(&node, 3);

	Frequency = freq; // set motor frequency

	p->print("\nSet freq = %d"); // for debugging
	p->print((int) ctr * delay);


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
	p->print("\n El:"); // for debugging
	p->print((int) ctr * delay);




	return atSetpoint;
}






int main(void)
{
	uint32_t sysTickRate;
	uint32_t psa = 30;

	/* Setup System */
	SystemCoreClockUpdate();
	Board_Init();
	Chip_Clock_SetSysTickClockDiv(1);
	sysTickRate = Chip_Clock_GetSysTickClockRate();
	SysTick_Config(sysTickRate / TICKRATE_HZ1);

	Chip_RIT_Init(LPC_RITIMER);
	I2CMaster I2C;
	i = &I2C;
	ITM_conv printer;
	p = &printer;

	DigitalIoPin b1(0, 29, true, true, true);
	DigitalIoPin b2(0, 9, true, true, true);
	DigitalIoPin b3(0, 10, true, true, true);


	DigitalIoPin a0(0, 8, false); 	// RS
	DigitalIoPin a1(1, 6, false);	// EN
	DigitalIoPin a2(1, 8, false); 	// D4
	DigitalIoPin a3(0, 5, false);	// D5
	DigitalIoPin a4(0, 6, false);	// D6
	DigitalIoPin a5(0, 7, false);	// D7

	LiquidCrystal lcd(&a0, &a1, &a2, &a3, &a4, &a5);
	lcd.begin(16, 2);
	lcd.setCursor(0, 1);

	Menu menu(&lcd, &b1, &b2, &b3, &Sleep);
	menu.updateDisplay();
	m = &menu;


	// Modbus
	LpcPinMap none = {-1, -1};
	LpcPinMap txpin = { 0, 18 }; // transmit pin that goes to debugger's UART->USB converter
	LpcPinMap rxpin = { 0, 13 }; // receive pin that goes to debugger's UART->USB converter
	LpcUartConfig cfg = { LPC_USART0, 115200, UART_CFG_DATALEN_8 | UART_CFG_PARITY_NONE | UART_CFG_STOPLEN_1, false, txpin, rxpin, none, none };
	LpcUart dbgu(cfg);
	Chip_SWM_MovablePortPinAssign(SWM_SWO_O, 1, 2); // Needed for SWO printf

	ModbusMaster node(2); // Create modbus object that connects to slave id 2
	node.begin(9600); // set transmission rate - other parameters are set inside the object and can't be changed here
	ModbusRegister ControlWord(&node, 0);
	ModbusRegister StatusWord(&node, 3);
	ModbusRegister OutputFrequency(&node, 102);
	ModbusRegister Current(&node, 103);

	// Startup
	ControlWord = 0x0406; // prepare for starting
	Sleep(1000); // give converter some time to set up
	ControlWord = 0x047F; // set drive to start mode
	Sleep(1000);



	while(1) {

		if(sensorCounter < 0){
			sensorCounter = 1000;
			uint8_t *val = I2C.ReadValueI2CM(3);
			p->print("\n Values: ");
			p->print(*val);
			val++;
			p->print(" - ");
			p->print(*val);
			val++;
			p->print(" - ");
			p->print(*val);

			psa = *val;
			menu.setPsa(psa);
			//printer.print(I2C.ReadValueI2CM(3));
		}

		menu.checkInputs();
		if(menu.hasNewValue()){
			if(menu.getMode()){
				setFrequency(node, menu.getSpeed()*200);
			}else{
				//Calculate Speed
				uint8_t calc = 50; // calculated value
				setFrequency(node, calc *200);
				menu.setSpeed(calc);
			}
			menu.clear();
			menu.updateDisplay();
		}


	}


	return 0;
}
