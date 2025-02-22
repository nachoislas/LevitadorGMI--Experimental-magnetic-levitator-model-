/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "muestras.h"
#include "fdacoefs.h"
#include "fda_derivador.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
volatile uint16_t counter = 0;
const int pwmGain = 800;
volatile int8_t sign = 1;
uint16_t numViL;

uint16_t tStart, tEnd, tTotal;

uint8_t printSerial = 0;
uint32_t tLast = 0;
const uint8_t printInterval = 20;		//20 ms

#define adcSamples 100
uint16_t adcBuf[adcSamples];

volatile uint16_t adcVal[2];
#define numSamples  adcSamples / 2

uint16_t adc_ch0[numSamples];
uint16_t adc_ch1[numSamples];

volatile uint8_t adcConverted = 0;
/*const uint16_t fs = 50000;
const float RL = 0.2;
const float L_4mm = 16.2e-3;
float ViL[numSamples];
float IL[numSamples];
float ILmed;
float deltaIL;
float derivadas[numSamples];
float Yestimada[numSamples];
float Yestimada_promedio;
*/

const int fixedPointGain = 1000;
const uint16_t fs = 50000;
const float RL = 0.2;
const float L_4mm = 16.2e-3;
const float Kh = 0.05;				//transductancia del sensor de efecto Hall
uint16_t vilIndex = 0;
#define N_ADC 16
const int n = N_ADC - 1;
uint16_t muestrasHall[N_ADC];
uint16_t muestrasRef[N_ADC];
int32_t muestrasADC[N_ADC];
int32_t IL[N_ADC];
int32_t ILmed;
float deltaIL;
int16_t derivadas[N_ADC];
int32_t derivadas_filtrada[N_ADC];
int32_t Yestimada[N_ADC];
int32_t Yestimada_filtrada[N_ADC];

void getViL(float * ViL, uint16_t * adc){
	float res = 3.3 / 4096;
	for(int i = 0; i < numSamples; i++){
		ViL[i] = adc[i] * res;
	}
}

float corriente(float Xn){
		return Xn / Kh;
}

float corriente_media(float * IL){
	float sum = 0;
	for(int i = 0; i<numSamples; i++){
		sum += IL[i];
	}
	return sum/numSamples;
}

float delta_corriente(float * IL){
	float ILmax = 0;
	float ILmin = 100;
	for(int i= 0; i<numSamples; i++){
		ILmax = max(IL[i], ILmax);
		ILmin = min(IL[i], ILmin);
	}
	return ILmax - ILmin;
}

int16_t derivar(uint16_t * muestrasHall){
	int16_t deriv = (muestrasHall[n] - muestrasHall[n - 1]);	//acá tengo la diferencia en el número que lee el adc
//	deriv += ILmed * 0.05 * RL / L_4mm;			//corrección I * R
	return deriv;
}

float estimar(int16_t derivada){
		float estim = abs(derivada) * 4.74e-3 - 4.55e-3;
		if(estim < 800e-3)
			return estim;
		else
			return 8e-3;						//esto lo hago para que la primera muestra no sea tan erronea
}

float filtrar(float * X, const float * B){			//acá la Y significa la salida del filtro, no la posición en mm
	float out = 0;
	for (int i = 0; i < N_ADC; i++) {				//acá aplico el filtro
		out += B[i] * X[n - i];			//B son los coeficientes de un filtro FIR
	 }
	 return out;
}

float promediar(float * Yestimada){
	float sum = 0;
	for(int i = 0; i < numSamples; i++){
		sum += Yestimada[i];
	}
	return sum / numSamples;
}

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  /*
  //esto es para generar una señal de prueba en pwm. Se puede anular esta parte cuando hacemos pruebas en la placa real
   HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
   HAL_TIM_Base_Start_IT(&htim4);
   //uint16_t loopDelay = 1;
    numViL = sizeof(ViLmuestras)/sizeof(ViLmuestras[0]);
    int loopCount = 0;
 */

   //cosas del adc
   HAL_TIM_Base_Start(&htim3);				//inicio el timer 3 para comenzar las conversiones del ADC
   HAL_ADC_Start_DMA(&hadc1, (uint32_t *) adcVal, 2);			//inicio adc dma
       	  	  	  	  	  	  	  	  	  	  	  	  	  	  	//para que lea la cantidad numSamples
   	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	//y luego dispare una interrupción
   //para medir el tiempo que tarda en ejecutar el código
   HAL_TIM_Base_Start(&htim4);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  if(adcConverted){
		 tStart = __HAL_TIM_GET_COUNTER(&htim4);		//para calcular el tiempo que tarda
		 adcConverted = 0;								//reseteo el flag del adc
		// HAL_TIM_Base_Stop(&htim3);			//paro el timer del adc (esto es temporal)
		// HAL_GPIO_TogglePin(led_GPIO_Port, led_Pin);		//esto es para ver la frecuencia de muestreo

		 //esto es para separar las muestras del canal 1 y el 2
		 //leo la muestra de corriente y la pongo al final del array del sensor de efecto hall
		  muestrasHall[n] = adcVal[0];
		  muestrasRef[n] = adcVal[1];

		  //convierto el valor del ADC en valor de tensión
		  //muestrasADC[n] = (muestrasHall[n] * 3.3 / 4096) / pwmGain;
		//  muestrasADC[n] = muestrasHall[n] * (8.056e-4 * fixedPointGain);	//3.3/4096 = 8.056e-4

		//  IL[n] = corriente(muestrasADC[n]);					//calculo el valor en corriente
		//  ILmed = (IL[n] + ILmed) / 2;							//añado al promedio movil
		  //deltaIL
		  derivadas[n]  = derivar(muestrasHall);			//calculo la derivada con la muestra anterior
		  //derivadas_filtrada[n] = filtrar(derivadas, Bderiv);	//filtro las derivadas obtenidas
		  Yestimada[n]  = estimar(derivadas[n]);
		  //Yestimada_filtrada[n] = filtrar(Yestimada, Bposicion);	//filtro

		  //acá paso todos los valores un lugar a la izquierda
		  //for (int i = n; i > 0; i--) {
				//muestrasHall[n - i] = muestrasHall[n - i + 1];
				//muestrasRef[n - i] = muestrasRef[n - i + 1];
				//muestrasADC[n - i] = muestrasADC[n - i + 1];
				//IL[n - i] = IL[n - i + 1];
				//derivadas[n - i] = derivadas[n - i + 1];
				//derivadas_filtrada[n - i] = derivadas_filtrada[n - i + 1];
				//Yestimada[n - i] = Yestimada[n - i + 1];
				//Yestimada_filtrada[n - i] = Yestimada_filtrada[n - i + 1];
		//	  }

		  muestrasHall[n - 1] = muestrasHall[n];

		  //esto es para graficar cada 20ms
	/*	  if(printSerial){
			  if(HAL_GetTick() - tLast > printInterval){
				  tLast = HAL_GetTick();
					 // uint16_t s1 = Yestimada[n] * 1e6;
					 // uint16_t s2 = Yestimada_filtrada[n] * 1e6;
					  uint16_t s1 = derivadas[n];
					  uint16_t s2 = derivadas_filtrada[n];
					 // uint16_t s1 = muestrasRef[n];
					 // uint16_t s1 = muestrasADC[n] * 1e6;
					 // uint16_t s2 = ILmed * 1e6;
					  char str[20];
					  sprintf(str, "%d,%d\r\n", s1, s2);
					 // sprintf(str, "%d\r\n", s1);
					  HAL_UART_Transmit(&huart1, (uint8_t * ) str, strlen(str), 100);
			  }
		  }
		  else if(++loopCount == 10000) {     //esto es para pausar el muestreo y graficar variables, después de cierta cantidad de muestras
			 for(uint8_t i = 0; i < N_ADC; i++){
				// uint16_t s1 = derivadas[i];
				 uint16_t s1 = muestrasHall[i];
				 char str[20];
				 // sprintf(str, "%d,%d\r\n", s1, s2);
				 sprintf(str, "%d\r\n", s1);
				 HAL_UART_Transmit(&huart1, (uint8_t * ) str, strlen(str), 100);
			 }
			 loopCount = 0;
		 }   */

		 tEnd = __HAL_TIM_GET_COUNTER(&htim4);
		 tTotal = tEnd - tStart;
		 HAL_GPIO_TogglePin(led_GPIO_Port, led_Pin);		//esto es para ver cuanto tarda en ejecutar el código
		// HAL_TIM_Base_Start(&htim3);
	 }

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 1 - 1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 256 - 1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 127;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 720 - 1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 2 - 1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 72 - 1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65536 - 1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : led_Pin */
  GPIO_InitStruct.Pin = led_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(led_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */







//callback para cuando finalizan las conversiones del ADC
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef * hadc){
	if(hadc->Instance == hadc1.Instance){
		adcConverted = 1;					//activo el flag del adc1
		// HAL_GPIO_TogglePin(led_GPIO_Port, led_Pin);		//esto es para ver la frecuencia de muestreo
	}
}












/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
