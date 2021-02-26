/*
 * hamamatsu.c
 *
 *  Created on: Sep 30, 2019
 *      Author: scass
 */
#include <stdio.h>
#include <string.h>
#include "hamamatsu.h"

const uint8_t H_COMMANDS[16][3] =
{
		{"HST"},
		{"HRT"},
		{"HPO"},
		{"HGS"},
		{"HGV"},
		{"HGC"},
		{"HGT"},
		{"HFI"},
		{"HGN"},
		{"HOF"},
		{"HON"},
		{"HRE"},
		{"HCM"},
		{"HSC"},
		{"HRC"},
		{"HBV"}
};

hps_t hv1;

uint8_t  checksum(uint8_t *buffer, uint8_t len)
{
	uint16_t seed=NULL;
	uint8_t * buf = (uint8_t *) buffer;
	for (uint8_t i = 0; i < len; ++i)
	seed += (unsigned int)(*buf++);
	sprintf((char*)buffer+len,"%02X",seed & 0x00ff); //agrega el checksum

	return seed & 0xff;
}

HAL_StatusTypeDef hps_sendcmd(UART_HandleTypeDef * uart, hps_cmd_t cmd,uint8_t * data,uint8_t data_len)
{	
	uint8_t cmd_buffer[32];
	uint8_t buffer_pos = 0;

	memset(cmd_buffer,0,sizeof(cmd_buffer));

	cmd_buffer[0] = 0x02; //STX
	buffer_pos++;

	
	memcpy(cmd_buffer+buffer_pos,H_COMMANDS[cmd],3);
	buffer_pos += 3;
	
	if ((data != NULL) && (data_len > 0))
	{
		memcpy(cmd_buffer+buffer_pos,data,data_len);
		buffer_pos += data_len;
	}

	cmd_buffer[buffer_pos] = 0x03;
	buffer_pos++;

	checksum(cmd_buffer, buffer_pos);
	buffer_pos+=2;

	cmd_buffer[buffer_pos] = 0x0d; // CR
	buffer_pos++;

	return HAL_UART_Transmit(uart, cmd_buffer, buffer_pos, 100);
}
