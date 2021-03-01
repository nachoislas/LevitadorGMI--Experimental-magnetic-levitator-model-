#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "stdio.h"

const char COM_INICIO[]= "INICIO";

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


comando_in comm_parse(uint8_t *data_buff)
{
comando_in dato;
	
char *letra="\n";
if(strstr((char*)data_buff,letra)!=NULL){
	
	/* check a command string */

	if(7==sscanf( (char*)data_buff, "INICIO,%f,%f,%f,%f,%f,%f,%f\r\n",&dato.coeficientes[0],&dato.coeficientes[1],&dato.coeficientes[2],&dato.coeficientes[3],&dato.coeficientes[5],&dato.coeficientes[6],&dato.coeficientes[7]))
		/**
		* @description: 
		* @params: CoefNumZ0,CoefNumZ1,CoefNumZ2,CoefDenZ0,CoefDenZ1,CoefDenZ2,GananciaInteg
		*/
	{	
		dato.name=INICIO;
	}
	else
	{
		dato.name=CMD_NULL;
	}
}
return dato;
}

