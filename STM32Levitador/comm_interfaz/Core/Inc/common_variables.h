/*
 * common_variables.h
 *
 *	En este archivo agrego todas las variables que puedan ser necesitadas desde distintos archivos.
 *  Created on: Mar 4, 2021
 *      Author: Javi
 */

#ifndef INC_COMMON_VARIABLES_H_
#define INC_COMMON_VARIABLES_H_

extern uint8_t enviarDatos;
//extern struct comp_t digitalComp;

//variables externas para lo del timer
extern TIM_HandleTypeDef htim4;
extern volatile uint8_t tim4_period_complete;

extern TIM_HandleTypeDef htim2;
extern volatile uint8_t tim2_period_complete;

//variables externas de la UART
extern  uint8_t uart1ReceivedData;
extern UART_HandleTypeDef huart1;
extern volatile uint8_t inputBuffer[64];
extern volatile uint8_t inputIndex;
extern volatile uint8_t uart_rx_complete;


//variables externas para el adc
extern volatile uint8_t adcConverted;
extern ADC_HandleTypeDef hadc1;

#endif /* INC_COMMON_VARIABLES_H_ */
