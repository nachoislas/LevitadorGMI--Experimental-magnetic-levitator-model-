/*
 * compensador.h
 *
 *  Created on: 12 mar. 2021
 *      Author: Javi
 */

#ifndef INC_COMPENSADOR_H_
#define INC_COMPENSADOR_H_

#include "main.h"
#include "common_variables.h"
#include <math.h>




//función para convertir los valores del adc en corriente en Amp
void leerCorriente(float * actual, float * anterior);
float convert_adc_en_corriente(uint16_t adcVal);
float estimar(float corr_actual, float corr_anterior);


#endif /* INC_COMPENSADOR_H_ */
