/*
 * hamamatsu.h
 *
 *  Created on: Sep 30, 2019
 *      Author: scass
 */

#ifndef HAMAMATSU_H_
#define HAMAMATSU_H_

#include "stm32f4xx_hal.h"

typedef enum
{
	HST = 0,
	HRT,
	HPO,
	HGS,
	HGV,
	HGC,
	HGT,
	HFI,
	HGN,
	HOF,
	HON,
	HRE,
	HCM,
	HSC,
	HRC,
	HBV
}hps_cmd_t;

typedef struct s_hps
{
	UART_HandleTypeDef * uart;

} hps_t;

//void hps_init(hps_t * new_hps, UART_HandleTypeDef * uart);
HAL_StatusTypeDef hps_sendcmd(UART_HandleTypeDef * uart, hps_cmd_t cmd,uint8_t * data,uint8_t data_len);

#endif /* HAMAMATSU_H_ */
