//archivo para poner los callbacks

#include "main.h"
#include "comm.h"
#include "string.h"

extern uint8_t uart1ReceivedData;
extern UART_HandleTypeDef huart1;
extern uint8_t inputBuffer[64];
extern uint8_t inputIndex;
extern comando_in comandoUart;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	char strConectado[] = "CONECTADO\r\n\0";
	HAL_UART_Transmit(&huart1, (uint8_t*) strConectado, strlen(strConectado), 100);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	inputBuffer[inputIndex] = uart1ReceivedData;
	inputIndex++;
	if(uart1ReceivedData == '\n'){
		inputBuffer[inputIndex] = '\0';
		uint8_t inputBufferLen = strlen((char*) inputBuffer);
		HAL_UART_Transmit(&huart1, inputBuffer, inputBufferLen, 100);
		comandoUart = comm_parse(inputBuffer);
		inputIndex = 0;
	}
	//HAL_UART_Receive_DMA(&huart1, &uart1ReceivedData, 1);
}

