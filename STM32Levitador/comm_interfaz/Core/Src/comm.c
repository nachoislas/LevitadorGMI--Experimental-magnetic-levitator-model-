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

comando_in comm_parse(uint8_t *data_buff){

	comando_in dato;
	char *letra="\n";
	if(strstr((char*)data_buff,letra)!=NULL){

		/* check a command string */
		if(7 == sscanf( (char*) data_buff,
						"INICIO,%f,%f,%f,%f,%f,%f,%f\r\n",
						&dato.coeficientes[0],
						&dato.coeficientes[1],
						&dato.coeficientes[2],
						&dato.coeficientes[3],
						&dato.coeficientes[5],
						&dato.coeficientes[6],
						&dato.coeficientes[7]
					  )
			)
		{
			dato.name=INICIO;
		}
		else
			dato.name=ETC;
	}
	else
		dato.name = CMD_NULL;
	return dato;
}
