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
#include <stdlib.h>



float obtenerCorrienteMedia(uint16_t * adc, uint16_t size);
void derivar(float * array, uint16_t * adc, uint16_t size, uint32_t fSample);
float promediar(float * array, uint16_t size);
float estimar(float derivada);
void compensar(comp_t * compensador);

#endif /* INC_COMPENSADOR_H_ */
