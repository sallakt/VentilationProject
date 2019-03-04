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
#include "LiquidCrystal.h"
#include "ritimer_15xx.h"
#include "I2CMaster.h"
#include "ITM_conv.h"

static volatile std::atomic_int counter;
#define TICKRATE_HZ1 (1000)
#define TASK (1)

#ifdef __cplusplus
extern "C" {
	#endif
	void SysTick_Handler(void)
	{
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



int main(void)
{
	uint32_t sysTickRate;
	uint32_t set[2] = {0, 0};
	uint32_t pos = 0;

	/* Setup System */
	SystemCoreClockUpdate();
	Board_Init();
	Chip_Clock_SetSysTickClockDiv(1);
	sysTickRate = Chip_Clock_GetSysTickClockRate();
	SysTick_Config(sysTickRate / TICKRATE_HZ1);

	Chip_RIT_Init(LPC_RITIMER);
	I2C_Master::I2C_Master I2C;
	ITM_conv printer;
	DigitalIoPin display(0, 8, false);	// A0
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

	lcd.setCursor(0, 0);
	lcd.print("Fan Speed: ");
	lcd.setCursor(0, 1);
	lcd.print("Pressure: ");
	while(1) {


#if TASK == 1
		if(b1.read()){
			Sleep(8);
			lcd.setCursor(13, pos);
			if(!b1.read()){
				set[pos]--;
				lcd.print(std::to_string(I2C.ReadValueI2CM(3)));
				printer.print(I2C.ReadValueI2CM(3));
			}
		}
		if(b2.read()){
			Sleep(8);
			lcd.setCursor(13, pos);
			if(!b2.read()){
				set[pos]--;
				lcd.print("42");
			}
		}
		if(b3.read()){
			Sleep(8);
			if(!b3.read()){
				if(pos == 1) pos = 0;
				if(pos == 0) pos = 1;
			}
		}
#endif


	}


	return 0;
}
