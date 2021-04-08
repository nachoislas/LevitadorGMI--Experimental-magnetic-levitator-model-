/*
 * comm.c
 *
 *  Created on: Mar 1, 2021
 *      Author: Javi
 */

#include "main.h"
#include "comm.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>



//función para parsear los comandos
comando_in comm_parse(volatile uint8_t *uart_buff){
	uint8_t data_len = strlen((char*) uart_buff);
	comando_in dato;

	char data_buff[data_len];
	memcpy(data_buff, (uint8_t*) uart_buff, data_len);		//copio la string al nuevo char array

		//nos fijamos si lo que recibimos tiene la estructura que sigue
	if(7 == sscanf(data_buff,
							"INICIO,%f,%f,%f,%f,%f,%f,%f\r\n",
							&dato.coeficientes[0],
							&dato.coeficientes[1],
							&dato.coeficientes[2],
							&dato.coeficientes[3],
							&dato.coeficientes[4],
							&dato.coeficientes[5],
							&dato.coeficientes[6]
						  )
			)
		{
			dato.name=INICIO;
		}

		else if(strstr(data_buff, "DETENER\r\n") != NULL){
			dato.name = DETENER;
		}

		else if(strstr(data_buff, "INTERVALO") != NULL){
			sscanf(data_buff, "INTERVALO,%hd\r\n", &dato.intervalo);
			dato.name = INTERVALO;
		}
		else
			dato.name=ETC;

	return dato;
}

void comm_case(comando_in comando_uart){

	switch ((uint8_t) comando_uart.name){
		case INICIO:
		{
			for(int i = 0; i<3; i++){
			digitalComp.numCoef[i] =	comando_uart.coeficientes[i];
			digitalComp.denCoef[i] = comando_uart.coeficientes[i+3];
			}
			digitalComp.Kint = comando_uart.coeficientes[6];
			enviarDatos = 1;				//para comenzar el envío de datos a la interfaz
			break;
		}
		case DETENER:
		{
			enviarDatos = 0;
		break;
		}
		case INTERVALO:
		{
			sendDataPeriod = comando_uart.intervalo;
			break;
		}
		case ETC:
		{
			serialSend(serialDevice,(uint8_t*) "HOLA GATO\r\n", strlen("HOLA GATO\r\n\0"), 100);
			break;
		}
		case CMD_NULL:
		{
			serialSend(serialDevice,(uint8_t*) "NULL\r\n", strlen("NULL\r\n\0"), 100);
			break;
		}
	}
}

//función que envía el comando conectado
void comm_send_conectado(){
	 char strConectado[] = "CONECTADO\r\n\0";
	 serialSend(serialDevice, (uint8_t*) strConectado, strlen(strConectado), 100);
}

//función para transmitir los datos leídos/calculados
void comm_send_data(int data1, int data2, int data3, int data4){
	char dataStr[32];
	sprintf(dataStr,"DATOS,%d,%d,%d,%d\r\n", data1, data2, data3, data4);
	//HAL_UART_Transmit(&huart1, (uint8_t*) dataStr, strlen(dataStr), 100);
	serialSend(serialDevice, (uint8_t*) dataStr, strlen(dataStr), 100);
}

//función para envíar datos por puerto serie, se puede elegir si es uart o usb
void serialSend(serialDevice_t device, uint8_t * buf, size_t bufLen, uint32_t timeOut){
	switch((uint8_t) device){
	case UART1:
				{
					HAL_UART_Transmit(&huart1, buf, bufLen, 100);
					break;
				}
	/*case UART2:
					{
						HAL_UART_Transmit(&huart2, buf, bufLen, 100);
						break;
					}
	case UART3:
						{
							HAL_UART_Transmit(&huart3, buf, bufLen, 100);
							break;
						} */
	case USB_SERIAL:
				{
					CDC_Transmit_FS(buf, bufLen);
					break;
				}
	}
}

//esta función es llamada desde el archivo usbd_cdc_if.c, en la función CDC_Receive_FS
void usbReceive(uint8_t * Buf, uint32_t * Len){
	memcpy( (uint8_t*) inputBuffer, Buf, *Len);
	inputBuffer[*Len] = '\0';
	uart_rx_complete = 1;
}
