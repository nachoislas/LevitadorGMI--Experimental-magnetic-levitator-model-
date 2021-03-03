#include "comm.h"



comando_in comm_parse(uint8_t *data_buff)
{
comando_in dato;
	
char *letra="\n";
if(strstr((char*)data_buff,letra)!=NULL){
	
	if(7==sscanf( (char*)data_buff, "INICIO,%f,%f,%f,%f,%f,%f,%f,%f\r\n",&dato.coeficientes[0],&dato.coeficientes[1],&dato.coeficientes[2],&dato.coeficientes[3],&dato.coeficientes[4],&dato.coeficientes[5],&dato.coeficientes[6],&dato.coeficientes[7]))
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


void comm_case(comando_in comando_uart)
/**
       * Accion segun comando reciido
       * @param 
       */

{	switch ((uint8_t) comando_uart.name)
						{
							case INICIO:
							{/************************************************
							 *  @description:
							 ***********************************************/
								
							break;
							}
							case ETC:
							{/************************************************
							 *  @description:
							 ***********************************************/
								
							break;
							}
							case CMD_NULL:
							{/************************************************
							 *  @description:
							 ***********************************************/
							break;
							}
						}
}

void lecturaADC(void)
/**
       * Leer datos del ADC
       * @param 
       */
{



}

void salidasDAC(void)
/**
       * Aplicar coeficientes
       * @param 
       */
{



}
