#ifndef __COMM_H
#define __COMM_H

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
	float coeficientes[8];
} comando_in;


comando_in comm_parse(uint8_t *data_buff);
void comm_case(comando_in comando_uart);
void salidasDAC(void);
void lecturaADC(void);
#endif
