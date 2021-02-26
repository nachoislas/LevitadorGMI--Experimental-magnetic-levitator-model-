//archivo para poner los callbacks

#include "main.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	//asd
}
