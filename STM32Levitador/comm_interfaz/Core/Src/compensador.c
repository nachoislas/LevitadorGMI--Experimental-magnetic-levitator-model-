/*
 * compensador.c
 *
 *  Created on: 12 mar. 2021
 *      Author: Javi
 */
#include "compensador.h"

float convert_adc_en_corriente(uint16_t adcVal){
	return adcVal * 3.3 / 4095;		//el 0.05 es la transconductancia del sensor de efecto hall
}

void leerSensorHall(float * actual, float * anterior){
	*anterior = convert_adc_en_corriente(adcBuf[0]);
	*actual = convert_adc_en_corriente(adcBuf[2]);  //acá sería el 2
}

float obtenerCorrienteMedia(uint16_t * adc, uint16_t size){
	float sum = 0;
	for(int i= 0; i< size; i++){
		sum += adc[i] * 3.3 / (4095 * size * 0.05);
	}
	return sum;
}

/*
float estimar(float Vcorr_actual, float Vcorr_anterior){
	//constantes del electroimán
	const uint8_t N = 150;
	const float mu0 = 4 * 3.1415 * 1e-7;
	const float Kh = 0.05;
	const float A = 25e-4;
	const uint8_t Vbus = 24;
	const float Linf = 12e-3;
	float estimacion;
	float derivada = (Vcorr_actual - Vcorr_anterior) * ADC_SAMPLE_FREQ;
	//float derivada = 1000 * (corr_actual - corr_anterior);
	estimacion =  mu0 * N * N * A * derivada / (2 * (Kh * Vbus -  Linf * derivada));
	if(estimacion < 0)
		estimacion *= -1;
	return estimacion;
}
*/

void derivar(float * array, uint16_t * adc, uint16_t size, uint32_t fSample){
	for(int i = 0; i < size - 1; i++){
		array[i] = abs((adc[i + 1] - adc[i]) * fSample * 3.3 / 4095);
		//array[i] = abs((adc[i + 1] - adc[i]));
	}
}

float promediar(float * array, uint16_t size){
	float sum = 0;
	for(int i = 0; i < size - 1; i++){
		sum += array[i] / size;
	}
	return sum;
}

float estimar(float derivada){
	//constantes del electroimán
	const uint8_t N = 150;
	const float mu0 = 4 * 3.1415 * 1e-7;
	const float Kh = 1;
	const float A = 25e-4;
	const uint8_t Vbus = 24;
	const float Linf = 12e-3;
	float estimacion;
	//float derivada = 1000 * (corr_actual - corr_anterior);
	estimacion =  mu0 * N * N * A * derivada / (2 * (Kh * Vbus -  Linf * derivada));
	if(estimacion < 0)
		estimacion *= -1;
	return estimacion;
}



