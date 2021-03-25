/*
 * compensador.c
 *
 *  Created on: 12 mar. 2021
 *      Author: Javi
 */
#include "compensador.h"

//promedia los valores del adc para obtener la corriente media del electroimán
float obtenerCorrienteMedia(uint16_t * adc, uint16_t size){
	float sum = 0;
	for(int i= 0; i< size; i++){
		sum += adc[i];
	}
	return sum * 3.3 / (4095 * size * 0.05);
}

//hace la derivada entre valores sucesivos
void derivar(float * array, uint16_t * adc, uint16_t size, uint32_t fSample){
	for(int i = 0; i < size - 1; i++){
		array[i] = abs((adc[i + 1] - adc[i]) * (float) fSample * 3.3 / 4095.0);
	}
}

//hace el promedio de las derivadas
float promediar(float * array, uint16_t size){
	float sum = 0;
	for(int i = 0; i < size - 1; i++){
		sum += array[i];
	}
	return sum/ (size - 1);
}

//estima la posición a partir de la derivada promedio
float estimar(float derivada){
	//constantes del electroimán
	/*const uint8_t N = 150;
	const float mu0 = 4 * 3.1415 * 1e-7;
	const float Kh = 1;
	const float A = 25e-4;
	const uint8_t Vbus = 24;
	const float Linf = 12e-3; */
	float estimacion;

	//usando estimación con formula alineal
	//estimacion =  mu0 * N * N * A * derivada / (2 * (Kh * Vbus -  Linf * derivada));

	//usando estimación con aproximación lineal por las mediciones que hicimos
	estimacion = (derivada - 7.75e2 )/ 1.7e5;

	if(estimacion < 0)
		estimacion *= -1;
	return estimacion;
}



