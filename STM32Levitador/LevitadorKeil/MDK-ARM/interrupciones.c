#include "main.h"
#include "comm.h"
#include <stdio.h>
#include <string.h>

extern 	UART_HandleTypeDef huart6;
extern 	DMA_HandleTypeDef hdma_usart6_rx;
extern	uint8_t rxData;
extern	uint8_t inputBuffer[20];
extern	uint8_t indice;
extern	comando_in comando_uart; 

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	char buf[] = "CONECTADO\r\n\0";
	HAL_UART_Transmit(&huart6,(uint8_t*) buf, strlen(buf), 100);
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
		HAL_UART_Transmit(&huart6, inputBuffer, indice, 100);
		inputBuffer[indice] = rxData;
		if(rxData == '\n'){
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
			indice = 0;
		}
		else{
				indice++;
				HAL_UART_Receive_IT(&huart6, &rxData, 1);
				}
}
