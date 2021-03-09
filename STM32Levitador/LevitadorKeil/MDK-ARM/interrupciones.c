#include "main.h"
#include "comm.h"
#include <stdio.h>
#include <string.h>

extern 	UART_HandleTypeDef huart6;
extern 	DMA_HandleTypeDef hdma_usart6_rx;
extern	uint8_t rxData;
extern	uint8_t inputBuffer[30];
extern	uint8_t indice;
extern	comando_in comando_uart; 
extern DAC_HandleTypeDef hdac;
extern ADC_HandleTypeDef hadc1;

extern const uint8_t adcSamples;  //puede ser otro numero
extern uint16_t adcBuf[1];


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	char buf[] = "CONECTADO\r\n\0";
	HAL_UART_Transmit(&huart6,(uint8_t*) buf, strlen(buf), 100);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
		inputBuffer[indice] = rxData;
		if(rxData == '\n'){
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
			indice = 0;
			comando_uart=comm_parse(inputBuffer);
			comm_case(comando_uart);
		}
		else{
				indice++;
				HAL_UART_Receive_IT(&huart6, &rxData, 1);
				}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef * hadc){
		//!Aca se tiene que pasar la funcion en z y los valores almaceados de los ADC
	//!y al final se pasa el valor de y[n]=algo; al adc
	//!AlgoMaximo es lo maximo que poriamos entregar con respecto a yn (5V?)
	/*const float algomaximo=5;
	float algo;
	uint16_t y = 4095 * algo/(algomaximo);
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R,y);
	*/
	//HAL_ADC_Stop_DMA(&hadc1);
	
	
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, 
		DAC_ALIGN_12B_R, 2000);
	/*double val = 0;
	uint32_t var = (uint32_t)(val*4096)/3.3;
	for (float m=0; m<=3.3; m=0.1+m)
	{
		var = 4095*m/3.3;
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, 
		DAC_ALIGN_12B_R, var);
		HAL_Delay(2000);
	}*/
	
	

//HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adcBuf, adcSamples);
}


