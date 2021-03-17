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

void leerCorriente(float * actual, float * anterior){
	*anterior = convert_adc_en_corriente(adcBuf[0]);
	*actual = convert_adc_en_corriente(adcBuf[2]);  //acá sería el 2
}

float estimar(float corr_actual, float corr_anterior){
	//constantes del electroimán
	const uint8_t N = 150;
	const float mu0 = 4 * 3.1415 * 1e-7;
	const float Kh = 0.05;
	const float A = 25e-4;
	const uint8_t Vbus = 24;
	const float Linf = 12e-3;
	float estimacion;
	float derivada = (corr_actual - corr_anterior) * ADC_SAMPLE_FREQ;
	//float derivada = 1000 * (corr_actual - corr_anterior);
	estimacion =  mu0 * N * N * A * derivada / (Kh * (2 * Vbus - 2 * Linf * derivada / Kh));
	if(estimacion < 0)
		estimacion *= -1;
	return estimacion;
}

