/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hamamatsu.h"
#include "hamamatsu.c"
#include "comm_protocol.h"
#include "comm_protocol.c"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

DAC_HandleTypeDef hdac;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_uart5_rx;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart6_rx;

osThreadId defaultTaskHandle;
osThreadId RxTaskHandle;
/* USER CODE BEGIN PV */
uint8_t new_char; 			//UART6 DMA
uint8_t new_char_uart3; //UART3 DMA
uint8_t new_char_uart5; //UART5 DMA

uint8_t uart3_buffer[64]; //buffer hv1
uint8_t uart3_buffer_index;

uint8_t uart5_buffer[64]; //buffer hv2
uint8_t uart5_buffer_index;

command_t command_input; 

uint32_t	PMT2_count_vv = 0;
uint32_t	PMT1_count_hh = 0;
uint32_t	PMT2_count_hv = 0;
uint32_t	PMT1_count_vh = 0;

uint32_t lv_h_prom = 0;
uint32_t lv_v_prom = 0;
uint32_t lh_h_prom = 0;
uint32_t lh_v_prom = 0;

float pol;
volatile uint16_t tim4_overflow;
volatile uint16_t tim5_overflow;
volatile bool counting = false;
uint8_t uart_tx_buffer[128];

float led_lvl;

float aux2;
uint16_t conversion;

uint16_t n = 50;	// cantidad de muestas
uint16_t dt = 20; 	//tiempo led on(Ventana)
uint8_t t_off = 10;//tiempo led off

bool record=0;
uint16_t debAux=0;

struct
{
	uint32_t lv_v_zero; //zero para canal V (Exitación V)
	uint32_t lv_h_zero; //zero para canal H (Exitación V)
	uint32_t lh_v_zero; //zero para canal V (Exitación H)
	uint32_t lh_h_zero;	//zero para canal h (Exitación H)
	float rv;
	float rh;
	float rv_rh;
	float pol;
	float g_factor;
	uint16_t led1_power;
	uint16_t led2_power;
	bool measuring;
	uint16_t 	hv1_power;
	uint16_t	hv2_power;
}polarimeter;

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_DAC_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM5_Init(void);
static void MX_TIM2_Init(void);
static void MX_UART5_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART6_UART_Init(void);
void StartDefaultTask(void const * argument);
void RXTaskHandler(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static float calc_polarization(uint32_t lv_count_h, uint32_t lv_count_v,uint32_t lh_count_h, uint32_t lh_count_v)
/**
       * Funcion devuelve el Calculo de la polarizacion.
       * @param rv an float argument.
       * @param rh a float argument.
       * @return The polarization value.
       */
{
	//if (lv_count_h <1 || lh_count_h <1 ) ESPERAR LA OTRA FUENTE
		//{return 0;}
	float rv = (float) lv_count_v/lv_count_h;
	float rh = (float) lh_count_v/lh_count_h;

	//if (rh == 0.0)
		//{return 0;}

	float rv_rh =  rv/rh;

	polarimeter.rv = rv;
	polarimeter.rh = rh;
	polarimeter.rv_rh = rv_rh;
	polarimeter.pol = (rv_rh-1)/(rv_rh+2);
		
	return (rv_rh-1)/(rv_rh+2);
}

int fputc(int ch, FILE *f) {
  if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
  return(ch);
}

float measure(uint16_t n, uint16_t dt, uint16_t t_off)
/**
			 * @description: Funcion que realiza el Conteo de Pulsos
       * @params: n = cantidad de muestras, dt: tiempo de conteo con leds on, toff: tiempos muertos
       */
/************************************************
 *  CONFIGURACIÓN CANALES
		LED 1 (DAC_CHANNEL_1) --> Exitación horizontal
		LED 2 (DAC_CHANNEL_2) --> Exitación vertical
		Canal horizontal: canal1 - htim4
		Canal vertical: canal2 - htim5
 ***********************************************/
{
	uint8_t btnSts= HAL_GPIO_ReadPin(GPIOA, COVER_Pin);
	if(btnSts==0){
	
		PMT1_count_vh=0;
		PMT1_count_hh=0;
		PMT2_count_hv=0;
		PMT2_count_vv=0;
		lv_h_prom = 0;
		lv_v_prom = 0;
		lh_h_prom = 0;
		lh_v_prom = 0;
		
		polarimeter.measuring = true;
		
		if (polarimeter.hv1_power==0 & polarimeter.hv2_power==0){
			hps_sendcmd(&huart3, HBV,(uint8_t*)"7B70", 4); //!Tension en 56V, definida para el pmt1 
			hps_sendcmd(&huart5, HBV,(uint8_t*)"7B70", 4); //!Tension en 56V, definida para el pmt2 
		} 
		for (uint16_t r = 0; r < 5;r++)
		{
			hps_sendcmd(&huart3, HON,NULL, 0); 	// Enciendo pmt1 
			HAL_Delay(10);
			hps_sendcmd(&huart5, HON,NULL, 0);	// Enciendo pmt2
			
		}	
		HAL_Delay(1000);
		if(polarimeter.led1_power==0){
			polarimeter.led1_power=4095;
		}
		if(polarimeter.led2_power==0){
				polarimeter.led2_power=4095;
		}
		
		for (uint16_t i = 0; i < n;i++)
		{
			//!CANAL 1
			
			//!Mido exitando con led horizontal (DAC 1)
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R,0);
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R,polarimeter.led1_power);
			//HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R,0);
			osDelay(t_off);
			//Enciendo Contadores
			HAL_TIM_Base_Start_IT(&htim4);
			HAL_TIM_Base_Start_IT(&htim5);
			//!Reinicio contadores y cuento en ventana con duracion dt
			tim4_overflow = 0;
			tim5_overflow = 0;
			__HAL_TIM_SetCounter(&htim4,0);
			__HAL_TIM_SetCounter(&htim5,0);
			osDelay(dt);
			//!Paro los contadores y guardo los valores
			HAL_TIM_Base_Stop_IT(&htim4);
			HAL_TIM_Base_Stop_IT(&htim5);
			PMT1_count_hh = __HAL_TIM_GetCounter(&htim4) + tim4_overflow * 65535;
			PMT2_count_hv = __HAL_TIM_GetCounter(&htim5) + tim5_overflow * 65535; 
			
			if (polarimeter.g_factor > 0)
				PMT2_count_hv *= polarimeter.g_factor; //!corrijo diferencia entre canales
			
			lh_h_prom += PMT1_count_hh;
			lh_v_prom += PMT2_count_hv;
			
			//!Canal2
			
			//! Mido exitando con led vertical (DAC 2)
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R,polarimeter.led2_power);
			//HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R,0);
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R,0);
			osDelay(t_off);
			
			//! Enciendo Contadores
			HAL_TIM_Base_Start_IT(&htim4);
			HAL_TIM_Base_Start_IT(&htim5);
			
			//! Reinicio contadores y cuento en ventana con duracion dt
			tim4_overflow = 0;
			tim5_overflow = 0;
			__HAL_TIM_SetCounter(&htim4,0);
			__HAL_TIM_SetCounter(&htim5,0);
			osDelay(dt);
			
			//!Paro los contadores y guardo los valores
			HAL_TIM_Base_Stop_IT(&htim4);
			HAL_TIM_Base_Stop_IT(&htim5);
			PMT1_count_vh = __HAL_TIM_GetCounter(&htim4) + tim4_overflow * 65535;
			PMT2_count_vv = __HAL_TIM_GetCounter(&htim5) + tim5_overflow * 65535; 
			
			if (polarimeter.g_factor > 0)
				PMT2_count_vv *= polarimeter.g_factor; //!corrijo diferencia entre canales
			
			lv_h_prom += PMT1_count_vh;
			lv_v_prom += PMT2_count_vv;
			
			record=1; //Para DEBUG
			osDelay(100);
			record=0; //Para debug
			debAux++;
			
		}
		//!Apago Fuentes
		hps_sendcmd(&huart3, HOF,NULL, 0); //!Desactiva salida de tension
		hps_sendcmd(&huart5, HOF,NULL, 0); //!Desactiva salida de tension
		//!Apago Leds
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R,0);
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R,0);
		//! Calculo promedios
		
		lv_h_prom /= n;
		lv_v_prom /= n;
		lh_h_prom /= n;
		lh_v_prom /= n;

		/*!Asignacion de Los conteos de Pulos a la estructura Polarimeter
		!*/

			polarimeter.lv_h_zero = lv_h_prom;
			polarimeter.lv_v_zero = lv_v_prom;
			polarimeter.lh_h_zero = lh_h_prom;
			polarimeter.lh_v_zero = lh_v_prom;

		polarimeter.measuring = false;
	}else if(btnSts==1){
		//uint8_t res[14]="$$RCOVER,1\r\n";
		//HAL_UART_Transmit(&huart6,res,sizeof(res),100);
		command_input.name=CMD_NULL;
	}
}

void data(command_t new_cmd)
/**
*@description: Accion a partir del comando recibido
*@description: Todos los comandos son de la siguiente forma: "$(Commando),valor1,valor2,valor3/r/n/n"
*@description: La respuesta si el comando fue recibido es la siguiente: "$(Commando),OK\r\n\n"
*@description: Los comandos que son consulta no poseen respuesta de comando recibido. Envian datos consultados.
*/
/************************************************
 *  CONFIGURACIÓN CANALES
		LED 1 (DAC_CHANNEL_1) --> Exitación horizontal
		LED 2 (DAC_CHANNEL_2) --> Exitación vertical
		Canal horizontal: canal1 - htim4
		Canal vertical: canal2 - htim5
 ***********************************************/
	{	switch ((uint8_t) new_cmd.name)
						{
				case CMD_CONFIG:
							{
								vTaskSuspend(RxTaskHandle);
								//!seteo led 1(horizontal)
								//uint16_t led_lvl = 4095 * new_cmd.params[0] / 100;
								led_lvl = 4095 * new_cmd.params[0] / 100;
								if (led_lvl>4095)
									led_lvl=4095;
								polarimeter.led1_power = led_lvl;
								HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R,led_lvl);
								
								//!seteo led 2
								led_lvl = 4095 * new_cmd.params[1] / 100;
								if (led_lvl>4095)
									led_lvl=4095;
								polarimeter.led2_power = led_lvl;
								HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, led_lvl);
								
								//!Apago Leds
								HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R,0);
								HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R,0);
								
								/************************************************
							 *  @descriptionSeteo Voltaje de Fuentes
							 ***********************************************/
								//*Fuente 1
								uint8_t aux[4];
								polarimeter.hv1_power=new_cmd.params[2];
								aux2 = polarimeter.hv1_power*0.55187;
								conversion=round(aux2);
								sprintf((char*)aux,"%02X", conversion & 0x0000ffff);
								hps_sendcmd(&huart3, HBV,aux, 4);
								HAL_Delay(100);
								//*Fuente Dos
								polarimeter.hv2_power=new_cmd.params[3];
								aux2 = polarimeter.hv2_power*0.55187;
								conversion=round(aux2);
								sprintf((char*)aux,"%02X", conversion & 0x0000ffff);
								hps_sendcmd(&huart5, HBV,aux, 4); 
								HAL_Delay(100);
								
								//!@description:Respuesta CONFIG Correcto
								uint8_t res[14]="$RCONFIG\r\n";
								HAL_UART_Transmit(&huart6,res,sizeof(res),100);
								
								command_input.name=CMD_NULL;
								vTaskResume(RxTaskHandle);
								break;
							}
							case READ_POL:
							{/************************************************
							 *  @description:Calculo de mili Polarizacion
							 ***********************************************/
								vTaskSuspend(RxTaskHandle);
								measure(n, dt, t_off);
								pol= calc_polarization(lv_h_prom-new_cmd.params[2],lv_v_prom-new_cmd.params[3],lh_h_prom-new_cmd.params[0],lh_v_prom-new_cmd.params[1]);
								uint8_t len = sprintf((char*)uart_tx_buffer,"$RPOL,%d\r\n",(int)(pol*1000.0));
								sprintf((char*)uart_tx_buffer,"$RPOL,%d\r\n",(int)(pol*1000.0));
								HAL_UART_Transmit(&huart6,(uint8_t*) uart_tx_buffer,len,100);
								
								command_input.name=CMD_NULL;
								vTaskResume(RxTaskHandle);
							break;
							}
							case READ_ZERO:
							{/************************************************
								*  @description:Calculo de valores para "Calibracion"(Conteo de pulsos sin muestraso patron)
							 ***********************************************/
							vTaskSuspend(RxTaskHandle);
							if (new_cmd.params_number >= 1)
							{
								/*Para debug cometado
								uint16_t n = new_cmd.params[0]; 	// cantidad de muestas
								uint16_t dt = new_cmd.params[1]; 	//tiempo led on
								uint8_t t_off = new_cmd.params[2];//tiempo led off
								*/
								
								measure(n, dt, t_off);
								command_input.name=CMD_NULL;
								vTaskResume(RxTaskHandle);
							}else
								{
								polarimeter.lh_h_zero = 0;
								polarimeter.lh_v_zero = 0;
								polarimeter.lv_h_zero = 0;
								polarimeter.lv_v_zero = 0;
								}
								uint8_t len = sprintf((char*)uart_tx_buffer,"$RZERO,%d,%d,%d,%d\r\n", polarimeter.lh_h_zero, polarimeter.lh_v_zero,
																								 polarimeter.lv_h_zero, polarimeter.lv_v_zero);
							
								HAL_UART_Transmit(&huart6,(uint8_t*) uart_tx_buffer,len,100);
								
								command_input.name=CMD_NULL;
								vTaskResume(RxTaskHandle);
							break;
							}
							case SET_GFACTOR:
							{/************************************************
							 *  Seteo Gfactor
							 ***********************************************/
								vTaskSuspend(RxTaskHandle);
								if (new_cmd.params_number > 0){
									polarimeter.g_factor = (float) new_cmd.params[0]/1000.0;
									uint8_t res[14]="$SGFAC,OK\r\n";
									HAL_UART_Transmit(&huart6,res,sizeof(res),100);
									command_input.name=CMD_NULL;
								}
								vTaskResume(RxTaskHandle);
								break;
							}
							case READ_COVER:
							{/************************************************
							 *  Estado de error(Envio comando de error)
							 *	@description: Respuesta a consulta tapa abierta o cerrada
							 ***********************************************/
								vTaskSuspend(RxTaskHandle);
								uint8_t btnSts= HAL_GPIO_ReadPin(GPIOA, COVER_Pin);
								if(btnSts==0){
									uint8_t word[12]="$RCOVER,0\r\n";
									HAL_UART_Transmit(&huart6,word,sizeof(word),100);
								}else if(btnSts==1){
									uint8_t word[12]="$RCOVER,1\r\n";
									HAL_UART_Transmit(&huart6,word,sizeof(word),100);
								}

								command_input.name=CMD_NULL;
								vTaskResume(RxTaskHandle);
								break;
							}
							case CMD_ERROR:
							{/************************************************
							 *  Estado de error(Envio comando de error)
							 ***********************************************/
								//!@description: Aviso Error a Recepcion de Comando
								vTaskSuspend(RxTaskHandle);
								uint8_t word[21]="$RABORT\r\n";
								HAL_UART_Transmit(&huart6,word,sizeof(word),100);
								command_input.name=CMD_NULL;
								vTaskResume(RxTaskHandle);
								break;
							}
							case CMD_RESET:
							{
								vTaskSuspend(RxTaskHandle);
								NVIC_SystemReset();
								vTaskResume(RxTaskHandle);
								break;
							}
							case CMD_NULL:
							{
							break;
							}
						}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
	if (huart->Instance == huart3.Instance)
	{
		uart3_buffer[uart3_buffer_index] = new_char_uart3;
		if (uart3_buffer_index < sizeof(uart3_buffer) - 1)
			uart3_buffer_index++;
	}
	else if (huart->Instance == huart5.Instance)
	{
		uart5_buffer[uart5_buffer_index] = new_char_uart5;
		if (uart5_buffer_index < sizeof(uart5_buffer) -1)
			uart5_buffer_index++;
	}
	else if (huart->Instance == huart6.Instance)
	{
		if (new_char == COM_CHAR)
		{
			memset(uart_cmd_buff,'\0',sizeof(uart_cmd_buff));
			uart_cmd_buff[0] = new_char;
			uart_cmd_buff_index =0;
		}
		else
		{
			if(uart_cmd_buff_index < UART_CMD_BUFFER_LENGTH)
				uart_cmd_buff_index++;
			else
				uart_cmd_buff_index = 0;
			
			uart_cmd_buff[uart_cmd_buff_index] = new_char;
		}
	}
	command_input = comm_parse(uart_cmd_buff);
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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_DAC_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM2_Init();
  MX_UART5_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  __HAL_UART_ENABLE_IT(&huart6,UART_IT_TC);
	HAL_UART_Receive_DMA (&huart6, &new_char, 1);
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0);
  HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0);
  HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
	hps_sendcmd(&huart3, HOF,NULL, 0);
	hps_sendcmd(&huart5, HOF,NULL, 0);

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of RxTask */
  osThreadDef(RxTask, RXTaskHandler, osPriorityNormal, 0, 128);
  RxTaskHandle = osThreadCreate(osThread(RxTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */
  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }
  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /** DAC channel OUT2 config
  */
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

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

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7200;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 10000;
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
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_EXTERNAL1;
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  sSlaveConfig.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING;
  sSlaveConfig.TriggerFilter = 0;
  if (HAL_TIM_SlaveConfigSynchro(&htim4, &sSlaveConfig) != HAL_OK)
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
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 0;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 65535;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_EXTERNAL1;
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  sSlaveConfig.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING;
  sSlaveConfig.TriggerFilter = 0;
  if (HAL_TIM_SlaveConfigSynchro(&htim5, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 38400;
  huart5.Init.WordLength = UART_WORDLENGTH_9B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_EVEN;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 38400;
  huart3.Init.WordLength = UART_WORDLENGTH_9B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_EVEN;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|PSU_CTRL_1_Pin|PSU_CTRL_2_Pin|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pins : BT1_Pin BT2_Pin BT3_Pin PSU_STS_1_Pin
                           PSU_STS_2_Pin */
  GPIO_InitStruct.Pin = BT1_Pin|BT2_Pin|BT3_Pin|PSU_STS_1_Pin
                          |PSU_STS_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 PSU_CTRL_1_Pin PSU_CTRL_2_Pin PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|PSU_CTRL_1_Pin|PSU_CTRL_2_Pin|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : COVER_Pin */
  GPIO_InitStruct.Pin = COVER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(COVER_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
	
  
	/* Infinite loop */
  for(;;)
  {
		data(command_input);
		
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_RXTaskHandler */
/**
* @brief Function implementing the RxTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_RXTaskHandler */
void RXTaskHandler(void const * argument)
{
  /* USER CODE BEGIN RXTaskHandler */
  /* Infinite loop */
  for(;;)
  {
		HAL_UART_Receive_DMA(&huart6,&new_char,1);
  }
  /* USER CODE END RXTaskHandler */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM10 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM10) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
