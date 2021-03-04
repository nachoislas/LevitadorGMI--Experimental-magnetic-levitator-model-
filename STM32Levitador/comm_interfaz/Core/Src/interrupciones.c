//archivo para poner los callbacks

#include "main.h"
#include "comm.h"
#include <string.h>

//variables externas para lo del timer
extern TIM_HandleTypeDef htim4;
extern volatile uint8_t tim4_period_complete;

extern TIM_HandleTypeDef htim2;
extern volatile uint8_t tim2_period_complete;

//variables externas de la UART
extern volatile uint8_t uart1ReceivedData;
extern UART_HandleTypeDef huart1;
extern volatile uint8_t inputBuffer[64];
extern volatile uint8_t inputIndex;
extern comando_in comandoUart;
extern volatile uint8_t uart_rx_complete;


//variables externas para el adc
extern volatile uint8_t adcConverted;
extern ADC_HandleTypeDef hadc1;

//callback para cuando los timers cumplen su ciclo
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == htim4.Instance){
		tim4_period_complete = 1;					//activo el flag del timer4
	}
	else if(htim->Instance == htim2.Instance){
			tim2_period_complete = 1;					//activo el flag del timer2
		}
}

//callback para cuando se recibe un byte en la UART
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	inputBuffer[inputIndex] = uart1ReceivedData;
	inputIndex++;
	if(uart1ReceivedData == '\n'){
		inputBuffer[inputIndex] = '\0';
		uart_rx_complete = 1;
		inputIndex = 0;
	}
}

//callback para cuando finalizan las conversiones del ADC
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef * hadc){
	if(hadc->Instance == hadc1.Instance){
		adcConverted = 1;					//activo el flag del adc1
	}
}

