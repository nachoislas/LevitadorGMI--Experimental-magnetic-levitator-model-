/*
 * comm.c
 *
 *  Created on: Mar 1, 2021
 *      Author: Javi
 */

#include "main.h"
#include "comm.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>



//función para parsear los comandos
comando_in comm_parse(volatile uint8_t *uart_buff){
	uint8_t data_len = strlen((char*) uart_buff);
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
		else if(strstr(data_buff, "DETENER\r\n") != NULL){
			dato.name = DETENER;
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

{
	switch ((uint8_t) comando_uart.name)
	{
		case INICIO:
		{/************************************************
		 *  @description:
		 ***********************************************/

			 /* char strCoef[100];
			  sprintf(strCoef, "%9.6f,%9.6f,%9.6f,%9.6f,%9.6f,%9.6f,%9.6f\r\n",
										  comp_coeff[0],
										  comp_coeff[1],
										  comp_coeff[2],
										  comp_coeff[3],
										  comp_coeff[4],
										  comp_coeff[5],
										  comp_coeff[6]
					 );  //9.6f para recibir float con 6 digitos decimales

			  //HAL_UART_Transmit(&huart1, (uint8_t*) strCoef, strlen(strCoef), 100); */

			 enviarDatos = 1;							//para comenzar el envío de datos a la interfaz
			 // HAL_TIM_Base_Start_IT(&htim2);
		break;
		}
		case DETENER:
				{/************************************************
				 *  @description:
				 ***********************************************/
					enviarDatos = 0;
					//HAL_TIM_Base_Stop_IT(&htim2);
				break;
				}
		case ETC:
				{/************************************************
				 *  @description:
				 ***********************************************/
					 HAL_UART_Transmit(&huart1,(uint8_t*) "HOLA GATO\r\n", strlen("HOLA GATO\r\n\0"), 100);
							 // comando_uart.name = CMD_NULL;

				break;
				}
		case CMD_NULL:
		{/************************************************
		 *  @description:
		 ***********************************************/
			HAL_UART_Transmit(&huart1,(uint8_t*) "NULL\r\n", strlen("NULL\r\n\0"), 100);
		break;
		}
	}
}

//función que envía el comando conectado
void comm_send_conectado(){
	 char strConectado[] = "CONECTADO\r\n\0";
	 HAL_UART_Transmit(&huart1, (uint8_t*) strConectado, strlen(strConectado), 100);
}

//función para transmitir los datos leídos/calculados
void comm_send_data(int data1, int data2, int data3, int data4){
	char dataStr[32];
	sprintf(dataStr,"DATOS,%d,%d,%d,%d\r\n", data1, data2, data3, data4);
	HAL_UART_Transmit(&huart1, (uint8_t*) dataStr, strlen(dataStr), 100);
}
