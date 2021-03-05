/*
 * time_functions.h
 *
 *  Created on: 5 mar. 2021
 *      Author: Javi
 */

#ifndef INC_TIME_FUNCTIONS_H_
#define INC_TIME_FUNCTIONS_H_

#include "main.h"

uint32_t millis(void);			//función que incrementa su valor cada 1 miliseg, como en arduino

//función que devuelve 1 o 0, dependiendo de si se transcurrió el periodo deseado
uint8_t checkPeriod(uint32_t period, uint32_t* pLast);

#endif /* INC_TIME_FUNCTIONS_H_ */
