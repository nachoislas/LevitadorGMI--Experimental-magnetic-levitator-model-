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


//esta función es llamada desde el main
void setAdcFreq(uint32_t fSample){

	const uint8_t period = 72;
	float  prescaler = 72.0E6/ (fSample * period);
	__HAL_TIM_SET_PRESCALER(&htim3, prescaler);
	__HAL_TIM_SET_AUTORELOAD(&htim3, period);
	/*&htim3.Init.Prescaler = (uint32_t) prescaler - 1;
	&htim3.Init.Period = period - 1;*/
}
