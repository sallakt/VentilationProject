/*
 * ITM_conv.cpp
 *
 *  Created on: Feb 26, 2019
 *      Author: vovan
 */
#include "ITM_conv.h"

void ITM_conv::print(int num)
{
	if(num >0)
	{
		snprintf(my_string, 100, "%d", num);
		ITM_write(my_string);
	}
}
void ITM_conv::print(std::string num)
{
	if(num != "0")
	{
		ITM_write(num.c_str());
	}
}

void ITM_conv::print(char str[])
{
	ITM_write(str);
}

