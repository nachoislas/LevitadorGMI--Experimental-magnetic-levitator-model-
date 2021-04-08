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
	/*
	for(int i = 0; i < size - 1; i++){
		array[i] = abs((adc[i + 1] - adc[i]) * (float) fSample * 3.3 / 4095.0);
	}*/

	float constant = 3.3 * fSample / 4095;
		for(int i = 0; i < size - 1; i++){
			float temp;
			temp = adc[i + 1] - adc[i];
			temp *= constant;
			array[i] = temp;
	//		array[i] = (adc[i + 1] - adc[i]) * fSample * 3.3 / 4095;
			if(array[i] < 0)
				array[i] *= -1;
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
	estimacion =  (derivada - 7.75e2 )/ 1.7e5;

	if(estimacion < 0)
		estimacion *= -1;
	return estimacion;
}


void compensar(comp_t * compensador){
	//cambio de lugar de n las variables
	compensador->error1_n1 = compensador->error1_n;
	compensador->control1_n1 = compensador->control1_n;
	compensador->error2_n1 = compensador->error2_n;
	compensador->error2_n2 = compensador->error2_n1;
	compensador->control2_n1 = compensador->control2_n;
	compensador->control2_n2 = compensador->control2_n1;


	//entradas
	int sum1_sign_in = compensador->sum1.sign_in;
	int sum1_sign_feed = compensador->sum1.sign_feed;
	int sum2_sign_in = compensador->sum2.sign_in;
	int sum2_sign_feed = compensador->sum2.sign_feed;
	float Kint = compensador->Kint;
	float Ts = compensador->Ts;
	float Yref = compensador->Yref;
	float Yestimada = compensador->Yestimada;

	float error1_n = compensador->error1_n;
	float error1_n1 = compensador->error1_n1;
	float error2_n = compensador->error2_n;
	float error2_n1 = compensador->error2_n1;
	float error2_n2 = compensador->error2_n2;
	float control1_n = compensador->control1_n;
	float control1_n1 = compensador->control1_n1;
	float control2_n = compensador->control2_n;
	float control2_n1 = compensador->control2_n1;
	float control2_n2 = compensador->control2_n2;

	float error2_arr[] = {error2_n, error2_n1, error2_n2};
	float control2_arr[] = {control2_n, control2_n1, control2_n2};
	float * numCoef = compensador->numCoef;
	float * denCoef = compensador->denCoef;
	//float salida= 0;
	//float error1 = estimada - ref;				//esto es considerando signos -+ en el sumador
	//salida = error1;

	//parte del integrador
	//hacemos la suma de la referencia con la estimación (considerando signos correctos)
	error1_n = Yref * sum1_sign_in + Yestimada * sum1_sign_feed;	//calcula el error1
	control1_n = control1_n1 + Kint * Ts * error1_n1;				//hace la integración. habría que ver si hay que poner un anti-windup

	//parte del segundo sumador con compensador por adelanto
	error2_n = control1_n * sum2_sign_in + Yestimada * sum2_sign_feed;	//calcula el error2
	for(int i=0; i<3; i++){
		control2_n += error2_arr[i] * numCoef[i] + control2_arr[i] * denCoef[i];
	}


	//cambio de lugar de n las variables
	compensador->error1_n = error1_n;
	compensador->error1_n1 = error1_n1;
	compensador->control1_n = control1_n;
	compensador->control1_n1 = control1_n1;
	compensador->error2_n = error2_n;
	compensador->error2_n1 = error2_n1;
	compensador->error2_n2 = error2_n2;
	compensador->control2_n = control2_n;
	compensador->control2_n1 = control2_n1;
	compensador->control2_n2 = control2_n2;

}



