#ifndef _COMM_PROTOCOL_H
#define _COMM_PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "stm32f4xx_hal.h"

#define UART_CMD_BUFFER_LENGTH 30
#define PARAMETERS_MAX 6 //5

#define COM_CHAR  '$'
#define COM_SEPARATOR_CHAR ','
#define COM_END_CHAR	'\n'

extern const char COM_READ_COUNTER[];
extern const char COM_READ_COUNTER_PROM[];
extern const char COM_READ_POLARIZATION_SINGLE[];
extern const char COM_READ_POLARIZATION_PROM[];
extern const char COM_READ_COUNTER_STAT[];
extern const char COM_SET_LED[];
extern const char COM_SET_ZERO[];

extern const char COM_SET_GFACTOR[];

typedef enum
{
	CMD_NULL = 0,	
	SET_GFACTOR,
	CMD_RESET,
	CMD_ERROR,
	READ_ZERO,
	CMD_CONFIG,
	READ_POL,
	READ_COVER
} command_name_t;

typedef struct command_s
{
	command_name_t	name;
	uint16_t params [PARAMETERS_MAX];
	uint8_t params_number;
} command_t;


extern uint8_t uart_cmd_buff[UART_CMD_BUFFER_LENGTH];
extern uint8_t uart_cmd_buff_index;

command_t comm_parse(uint8_t *data_buff);

#endif
