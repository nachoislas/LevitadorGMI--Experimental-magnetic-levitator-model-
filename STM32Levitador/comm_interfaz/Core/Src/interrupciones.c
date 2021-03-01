//archivo para poner los callbacks

#include "main.h"
#include "string.h"

extern uint8_t uart1ReceivedData;
extern UART_HandleTypeDef huart1;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	char strConectado[] = "CONECTADO\r\n\0";
	HAL_UART_Transmit(&huart1, (uint8_t*) strConectado, strlen(strConectado), 100);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	HAL_UART_Transmit(&huart1, &uart1ReceivedData, 1, 100);
	HAL_UART_Receive_DMA(&huart1, &uart1ReceivedData, 1);
}

