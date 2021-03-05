//archivo para poner los callbacks

#include "main.h"
#include "comm.h"
#include "common_variables.h"
#include <string.h>

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

