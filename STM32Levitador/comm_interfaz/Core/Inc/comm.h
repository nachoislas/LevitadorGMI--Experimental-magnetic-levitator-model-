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
#include "usb_device.h"
#include "usbd_cdc_if.h"

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
	INTERVALO,
	ETC
} comando_nombre;

typedef struct command_s
{
	comando_nombre name;
	float coeficientes[7];
	int16_t intervalo;
} comando_in;

comando_in comm_parse(volatile uint8_t *data_buff);		//parsea los comandos
extern comando_in comandoUart;
void comm_case(comando_in comando_uart);				//decide qué hacer dependiendo del comando
void comm_send_conectado();								//función para envíar el comando conectado
void comm_send_data(int,int,int,int);					//función para envíar valores del adc

//función para envíar datos por puerto serie, se puede elegir si es uart o usb
void serialSend(serialDevice_t device, uint8_t * buf, size_t bufLen, uint32_t timeOut);
void usbReceive(uint8_t * Buf, uint32_t * Len);   //función llamada cuando se reciben datos por usb

#endif /* INC_COMM_H_ */
