#ifndef _COMM_H
#define _COMM_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "stdio.h"

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


//comando_in comm_parse(uint8_t *data_buff);

#endif
