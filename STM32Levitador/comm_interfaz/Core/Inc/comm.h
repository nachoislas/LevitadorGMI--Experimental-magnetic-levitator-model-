/*
 * comm.h
 *
 *  Created on: Mar 1, 2021
 *      Author: Javi
 */

#ifndef INC_COMM_H_
#define INC_COMM_H_
#include "main.h"
#include "common_variables.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define UART_CMD_BUFFER_LENGTH 64
#define PARAMETERS_MAX 7

#define COM_CHAR  '$'
#define COM_SEPARATOR_CHAR ','
#define COM_END_CHAR	'\n'



typedef enum
{
	CMD_NULL = 0,
	INICIO,
	DETENER,
	ETC
} comando_nombre;

typedef struct command_s
{
	comando_nombre name;
	float coeficientes[7];
} comando_in;

comando_in comm_parse(volatile uint8_t *data_buff);
extern comando_in comandoUart;
void comm_case(comando_in comando_uart);
void comm_send_conectado();
void comm_send_data(int,int,int,int);

#endif /* INC_COMM_H_ */
