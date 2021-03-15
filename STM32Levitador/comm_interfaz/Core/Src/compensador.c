/*
 * compensador.c
 *
 *  Created on: 12 mar. 2021
 *      Author: Javi
 */
#include "compensador.h"

float convert_adc_en_corriente(uint16_t adcVal){
	return adcVal * 3.3 / (4095 * 0.05);		//el 0.05 es la transconductancia del sensor de efecto hall
}

void leerCorriente(float * actual, float * anterior, float * media){
	*anterior = convert_adc_en_corriente(adcBuf[0]);
	*actual = convert_adc_en_corriente(adcBuf[2]);  //acá sería el 2
	*media = (*actual + *anterior + *media) / 3;
}

float estimar(float corr_actual, float corr_anterior){
	float derivada = (corr_actual - corr_anterior) * ADC_SAMPLE_FREQ;
	//float derivada = 1000 * (corr_actual - corr_anterior);

	return derivada;
}

