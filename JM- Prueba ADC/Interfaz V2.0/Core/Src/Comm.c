#include "Comm.h"
#include <string.h>
#include <stdio.h>

extern UART_HandleTypeDef huart3;
extern TIM_HandleTypeDef htim2;

//extern uint8_t enviarDatos;

//función para parsear los comandos

comando_in comm_parse(uint8_t *uart_buff)
{
	uint8_t data_len = strlen((char*) uart_buff);
	comando_in dato;

	char data_buff[data_len];
	memcpy(data_buff, uart_buff, data_len);   //copio el string al nuevo char array

	//Nos fijamos si lo que recibe tiene la estructura que sigue:

	if (7 == sscanf(data_buff,
							"INICIO,%f,%f,%f,%f,%f,%f,%f\r\n",
							&dato.coeficientes[0],
							&dato.coeficientes[1],
							&dato.coeficientes[2],
							&dato.coeficientes[3],
							&dato.coeficientes[4],
							&dato.coeficientes[5],
							&dato.coeficientes[6]))
	{
		dato.name = INICIO;
	}

	else if (strstr(data_buff,"DETENER\r\n") != NULL)
	{
		dato.name = DETENER;
	}
	else
	{
		dato.name = ETC;
	}

	return dato;

}


void comm_case(comando_in comando_uart)
{

	switch ((uint8_t) comando_uart.name)
	{
		case INICIO:
		{
			float comp_coeff[7];
				memcpy(comp_coeff, comando_uart.coeficientes, 7 * sizeof(comp_coeff));
				char strCoef[100];

				sprintf(strCoef, "%9.6f,%9.6f,%9.6f,%9.6f,%9.6f,%9.6f,%9.6f\r\n",
															comp_coeff[0],
															comp_coeff[1],
															comp_coeff[2],
															comp_coeff[3],
															comp_coeff[4],
															comp_coeff[5],
															comp_coeff[6]);

				HAL_UART_Transmit(&huart3, (uint8_t*) strCoef, strlen(strCoef), 100);

			comando_uart.name = CMD_NULL;

			break;
		}

		case DETENER:
		{
			HAL_UART_Transmit(&huart3,(uint8_t*) "Entro en DETENER\r\n", strlen("Entro en DETENER\r\n\0"), 100);
			comando_uart.name = CMD_NULL;
			break;
		}

		case ETC:
		{
			HAL_UART_Transmit(&huart3,(uint8_t*) "HOLA GATO\r\n", strlen("HOLA GATO\r\n\0"), 100);
			comando_uart.name = CMD_NULL;
			break;
		}

		case CMD_NULL:
		{
			HAL_UART_Transmit(&huart3,(uint8_t*) "NULL\r\n", strlen("NULL\r\n\0"), 100);
			break;
		}

	}

}



void comm_send_conectado()
{
	char strConectado[] = "CONECTADO\r\n\0";
	HAL_UART_Transmit(&huart3,(uint8_t*) strConectado, strlen(strConectado), 100);
}















