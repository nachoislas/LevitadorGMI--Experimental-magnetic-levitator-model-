/*
 * time_functions.c
 *
 *  Created on: 5 mar. 2021
 *      Author: Javi
 */

#include "time_functions.h"

uint32_t millis(){
	return HAL_GetTick();
}


uint8_t checkPeriod(uint32_t period, uint32_t* pLast){
	uint32_t tNow = millis();
	if(tNow - *pLast > period) {
		*pLast = tNow;
		return 1;
	}
	else
		return 0;
}
