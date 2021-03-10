#include "main.h"
#include "string.h"


extern UART_HandleTypeDef huart3;
extern ADC_HandleTypeDef hadc1;
extern volatile uint8_t inputBuffer[64];
extern volatile uint8_t inputIndex;
extern volatile uint8_t uart3ReceivedData;
extern volatile uint8_t uart_rx_complete;
extern volatile uint8_t tim2_period_complete;

extern volatile uint8_t adcConverted;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	 tim2_period_complete = 1;					//activo el flag del timer2 --> para enviar "CONECTADO"
}


//Callback para cuando se recibe un byte en la UART
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	inputBuffer[inputIndex] = uart3ReceivedData;
	inputIndex++;
	if (uart3ReceivedData == '\n')
	{
		inputBuffer[inputIndex] = '\0';
		uart_rx_complete = 1;
		inputIndex = 0;
	}
}


//callback para cuando finalizan las conversiones del ADC
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef * hadc)
{
	if(hadc->Instance == hadc1.Instance){
		adcConverted = 1;					//activo el flag del adc1
}

















}
