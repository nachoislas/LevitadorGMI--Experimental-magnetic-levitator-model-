/*
 * comm.c
 *
 *  Created on: Mar 1, 2021
 *      Author: Javi
 */

#include "comm.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

comando_in comm_parse(uint8_t *uart_buff, uint8_t data_len){

	comando_in dato;
	char data_buff[data_len];
	memcpy(data_buff, uart_buff, data_len);		//copio la string al nuevo char array

		//nos fijamos si lo que recibimos tiene la estructura que sigue
	if(7 == sscanf((char*) uart_buff,
							"INICIO,%f,%f,%f,%f,%f,%f,%f\r\n",
							&dato.coeficientes[0],
							&dato.coeficientes[1],
							&dato.coeficientes[2],
							&dato.coeficientes[3],
							&dato.coeficientes[4],
							&dato.coeficientes[5],
							&dato.coeficientes[6]
						  )
			)
		{
			dato.name=INICIO;
		}
		else
			dato.name=ETC;
	return dato;
}
