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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "muestras_corriente.h"
#include "muestras_int.h"
#include "fdacoefs.h"
#include "fda_derivador.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t printSerial = 0;
uint8_t printDataFrame = 0;
uint16_t tStart, tEnd, tTotal;

#define numSamples  50
const uint16_t fs = 50000;
const float alfa = 0.4665119;			//el alfa sale de alfa = 1 - exp(-2pi*fc/fs) fc es la frecuencia de corte
const float RL = 0.2;
const float L_4mm = 16.2e-3;
const float Kh = 0.05;				//transductancia del sensor de efecto Hall
uint16_t vilIndex = 0;
#define N_ADC 16
const int n = N_ADC - 1;
uint16_t muestrasHall[N_ADC];
float IL[N_ADC];
float ILmed;
float deltaIL;
int16_t derivadas[N_ADC];
float derivadas_filtrada[N_ADC];
float Yestimada[N_ADC];
float Yestimada_1[N_ADC];
float Yestimada_filtrada[N_ADC];

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

float delta_corriente(float  IL){
	static float ILmax = 0;
	static float ILmin = 100;
	ILmax = max(IL, ILmax);
	ILmin = min(IL, ILmin);
	return ILmax - ILmin;
}

int16_t derivar(uint16_t * muestrasHall){
	int16_t deriv = (muestrasHall[n] - muestrasHall[n - 1]);	//acá tengo la diferencia en el número que lee el adc
//	deriv += ILmed * 0.05 * RL / L_4mm;			//corrección I * R
	return deriv;
}

float estimar(int16_t derivada){
		float estim = abs(derivada) * 2.37e-3 - 4.55e-3;
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
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  //para medir el tiempo que tarda en ejecutar el código
    HAL_TIM_Base_Start(&htim4);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  tStart = __HAL_TIM_GET_COUNTER(&htim4);		//para calcular el tiempo que tarda
	  //leo la muestra de corriente y la pongo al final del array del ADC
	  muestrasHall[n] = ViL[vilIndex];
	 // IL[n] = corriente(muestrasHall[n]);			//calculo el valor en corriente
	//  ILmed = alfa * IL[n] + (1 - alfa) * ILmed;		//exponential integrator
	  //  ILmed = (IL[n] + ILmed) / 2;							//añado al promedio movil
	  //deltaIL
	  derivadas[n]  = derivar(muestrasHall);		//acá calculo derivada y hago el valor absoluto
	  //derivadas_filtrada[n] = filtrar(derivadas, Bderiv);	//filtro las derivadas obtenidas
	  //Yestimada_1[n] = estimar(derivadas[n]);				//esto es para comparar con la filtrada
	  Yestimada[n]  = estimar(derivadas[n]);		//estimacion
	  //Yestimada_filtrada[n] = filtrar(Yestimada, Bposicion);	//filtro

	  //acá paso todos los valores un lugar a la izquierda
	/*  for (int i = n; i > 0; i--) {
	 		muestrasHall[n - i] = muestrasHall[n - i + 1];
	 		IL[n - i] = IL[n - i + 1];
	 		derivadas[n - i] = derivadas[n - i + 1];
	 		derivadas_filtrada[n - i] = derivadas_filtrada[n - i + 1];
	 		Yestimada[n - i] = Yestimada[n - i + 1];
	 		Yestimada_1[n - i] = Yestimada_1[n - i + 1];
	 		Yestimada_filtrada[n - i] = Yestimada_filtrada[n - i + 1];
	 	  } */
	  muestrasHall[n-1] = muestrasHall[n];


	  if(printSerial){
		// uint16_t s1 = Yestimada[n] * 1e6;
		// uint16_t s2 = Yestimada_filtrada[n] * 1e6;
		 // int16_t s1 = derivadas[n];
		 // int16_t s2 = abs(derivadas[n]);
		 // int16_t s2 = derivadas_filtrada[n];
		 // uint16_t s1 = IL[n] * 1e3;
		 // uint16_t s2 = ILmed * 1e3;
		  char str[20];
		  //sprintf(str, "%d,%d\r\n", s1, s2);
		 sprintf(str, "%d\r\n,", muestrasHall[n]);
		  HAL_UART_Transmit(&huart1, (uint8_t * ) str, strlen(str), 100);
	  }
	  else if(printDataFrame){
		  if(vilIndex == 0){
			  char str1[64];
			  sprintf(str1,"muestras,derivadas,estimacion\r\n");
			  HAL_UART_Transmit(&huart1, (uint8_t*) str1, strlen(str1), 100);
		  }
		  uint16_t s1 = muestrasHall[n];
		  int16_t s2 = derivadas[n];
		  uint16_t s3 = Yestimada[n] * 1e6;
		  char str2[64];
		  sprintf(str2, "%d,%d,%d\r\n", s1,s2,s3);
		  HAL_UART_Transmit(&huart1, (uint8_t*) str2, strlen(str2), 100);
	  }

	  //incremento el index y vuelvo a empezar a recorrer el arreglo de ViL si es necesario
	  vilIndex++;
	  if(vilIndex == 302)
		  vilIndex = 0;
	  tEnd = __HAL_TIM_GET_COUNTER(&htim4);
	  tTotal = tEnd - tStart;
	  HAL_Delay(30);
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
  htim4.Init.Period = 65535;
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

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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
