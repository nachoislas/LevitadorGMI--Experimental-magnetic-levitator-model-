/*
 * comm.h
 *
 *  Created on: Mar 1, 2021
 *      Author: Javi
 */

#ifndef INC_COMM_H_
#define INC_COMM_H_
#include "main.h"
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
	ETC
} comando_nombre;

typedef struct command_s
{
	comando_nombre name;
	float coeficientes[7];
} comando_in;

comando_in comm_parse(uint8_t *data_buff, uint8_t data_len);

#endif /* INC_COMM_H_ */
