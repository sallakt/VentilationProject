/*
 * ITM_conv.h
 *
 *  Created on: Feb 26, 2019
 *      Author: vovan
 */

#ifndef ITM_CONV_H_
#define ITM_CONV_H_
#include <string>
#include "ITM_write.h"


class ITM_conv
{
private:
	char my_string[100];

public:
	ITM_conv(){ITM_init(); }
	virtual ~ITM_conv(){}
	void print(int num);
	void print(std::string num);
	void print(char str[]);
};

#endif /* ITM_CONV_H_ */
