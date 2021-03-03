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

extern UART_HandleTypeDef huart1;

//función para parsear los comandos
comando_in comm_parse(uint8_t *uart_buff, uint8_t data_len){

	comando_in dato;
	char data_buff[data_len];
	memcpy(data_buff, uart_buff, data_len);		//copio la string al nuevo char array

		//nos fijamos si lo que recibimos tiene la estructura que sigue
	if(7 == sscanf(data_buff,
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

void comm_case(comando_in comando_uart)
/**
       * Accion segun comando reciido
       * @param
       */

{	switch ((uint8_t) comando_uart.name)
						{
							case INICIO:
							{/************************************************
							 *  @description:
							 ***********************************************/
								 float comp_coeff[7];
										  memcpy(comp_coeff, comando_uart.coeficientes,  7 * sizeof(*comp_coeff));
										  char strCoef[100];
										  sprintf(strCoef, "%9.6f,%9.6f,%9.6f,%9.6f,%9.6f,%9.6f,%9.6f\r\n",
												  	  	  	  	  	  comp_coeff[0],
																	  comp_coeff[1],
																	  comp_coeff[2],
																	  comp_coeff[3],
																	  comp_coeff[4],
																	  comp_coeff[5],
																	  comp_coeff[6]
												 );  //9.6f para recibir float con 6 digitos decimales

										  HAL_UART_Transmit(&huart1, (uint8_t*) strCoef, strlen(strCoef), 100);
										  comando_uart.name = CMD_NULL;
							break;
							}
							case ETC:
							{/************************************************
							 *  @description:
							 ***********************************************/
								 HAL_UART_Transmit(&huart1,(uint8_t*) "HOLA GATO\r\n", strlen("HOLA GATO\r\n\0"), 100);
										  comando_uart.name = CMD_NULL;

							break;
							}
							case CMD_NULL:
							{/************************************************
							 *  @description:
							 ***********************************************/
							break;
							}
						}
}
