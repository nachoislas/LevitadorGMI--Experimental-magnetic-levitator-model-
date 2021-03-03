//archivo para poner los callbacks

#include "main.h"
#include "comm.h"
#include <string.h>

//variables externas para lo del timer
extern TIM_HandleTypeDef htim4;

//variables externas de la UART
extern uint8_t uart1ReceivedData;
extern UART_HandleTypeDef huart1;
extern uint8_t inputBuffer[64];
extern uint8_t inputIndex;
extern comando_in comandoUart;
extern uint8_t uart_rx_complete;
extern uint8_t inputBufferLen;

//variables externas para el adc
extern uint8_t adcConverted;

//callback para cuando el timer 4 cumple su ciclo
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == htim4.Instance){
		char strConectado[] = "CONECTADO\r\n\0";
		HAL_UART_Transmit(&huart1, (uint8_t*) strConectado, strlen(strConectado), 100);
	}
}

//callback para cuando se recibe un byte en la UART
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	inputBuffer[inputIndex] = uart1ReceivedData;
	inputIndex++;
	if(uart1ReceivedData == '\n'){
		inputBuffer[inputIndex] = '\0';
		inputBufferLen = strlen((char*) inputBuffer);

		//HAL_UART_Transmit(&huart1, inputBuffer, inputBufferLen, 100);

		//comandoUart = comm_parse(inputBuffer, inputBufferLen);
		uart_rx_complete = 1;
		inputIndex = 0;
	}
}

//callback para cuando finalizan las conversiones del ADC
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef * hadc){
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	adcConverted = 1;
}

