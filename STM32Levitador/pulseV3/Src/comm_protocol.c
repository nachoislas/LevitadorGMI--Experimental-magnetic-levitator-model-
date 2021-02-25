#include <string.h>
#include "comm_protocol.h"

const char COM_READ_ZERO[]= "RZERO";
const char COM_RESET[]= "RESET";
const char COM_SET_GFACTOR[]= "SGFACTOR";
const char COM_RPOL[]= "RPOL";
const char COM_CONFIG[]= "RCONFIG";
const char COM_COVER[]="RCOVER";

extern osThreadId defaultTaskHandle;
extern osThreadId RxTaskHandle;

uint8_t uart_cmd_buff[UART_CMD_BUFFER_LENGTH];
uint8_t uart_cmd_buff_index;

uint8_t debug = 0;

command_t comm_parse(uint8_t *data_buff)
{
	command_t parsed;
	parsed.name = CMD_NULL;
	parsed.params_number = 0;
	parsed.params[4]=0;
	parsed.params[5]=0;

char *letra="\n";
if(strstr(data_buff,letra)!=NULL){

	
	/* check a command string */
	if(4==sscanf( data_buff, "$RPOL,%d,%d,%d,%d\r\n", &parsed.params[0], &parsed.params[1], &parsed.params[2],&parsed.params[3]))
		/**
		* @description: Funcion para calcular Polarizacion
		* @params: parsed.params[0] = offset HV, parsed.params[1]: offset HH, parsed.params[2]: offset VV,parsed.params[3]: offset VH
		*/
	{	parsed.name = READ_POL;
		data_buff += strlen(COM_RPOL);
	}else if (4==sscanf( data_buff, "$RCONFIG,%d,%d,%d,%d\r\n", &parsed.params[0], &parsed.params[1], &parsed.params[2],&parsed.params[3]))
		/**
		* @description: Funcion para calcular 
		* @params: parsed.params[0] = config LED HORIZONTAL, parsed.params[1]: config LED VERTICAL, parsed.params[2]: config FUENTE1,parsed.params[3]: config FUENTE2
		*/
	{	parsed.name = CMD_CONFIG;
		data_buff += strlen(COM_CONFIG);
	}else if (3==sscanf( data_buff, "$RZERO,%d,%d,%d\r\n", &parsed.params[0], &parsed.params[1], &parsed.params[2]))
		/**
		* @description: Funcion para calcular 
		* @params: parsed.params[0] = cantidad muestras, parsed.params[1]: Tiempo leds on y medicion, parsed.params[2]: tiempos muertos
		*/
	{	data_buff += strlen(COM_READ_ZERO);
		parsed.name = READ_ZERO;
	} else if (1==sscanf( data_buff, "$SGFACTOR,%d\r\n"))
		/**
		* @description: Funcion para calcular 
		* @params: parsed.params[0] = Factor de Correccion
		*/
	{	data_buff += strlen(COM_SET_GFACTOR);
		parsed.name = SET_GFACTOR;
	} else if (0 == strncmp(data_buff, "$RRESET\r\n",strlen("$RRESET\r\n")))
		/**
		* @description: Funcion para Reset 
		*/
	{
		data_buff += strlen(COM_RESET);
		parsed.name = CMD_RESET;
	}else if (0 == strncmp(data_buff, "$RCOVER",6))
		/**
		* @description: Consulta tapa abierta/cerrada
		*/
	{
		data_buff += strlen(COM_COVER);
		parsed.name = READ_COVER;
	}
	else
	{
		parsed.name = CMD_ERROR;
	}
	if (parsed.params[0]>0){
				parsed.params_number = 1;
		}
}
return parsed;
}

