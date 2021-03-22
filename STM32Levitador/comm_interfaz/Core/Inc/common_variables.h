/*
 * common_variables.h
 *
 *	En este archivo agrego todas las variables que puedan ser necesitadas desde distintos archivos.
 *  Created on: Mar 4, 2021
 *      Author: Javi
 */

#ifndef INC_COMMON_VARIABLES_H_
#define INC_COMMON_VARIABLES_H_

extern uint8_t enviarDatos;
//extern struct comp_t digitalComp;

//variables externas para lo del timer
extern TIM_HandleTypeDef htim4;
extern volatile uint8_t tim4_period_complete;

extern TIM_HandleTypeDef htim2;
extern volatile uint8_t tim2_period_complete;
extern int16_t sendDataPeriod;

extern TIM_HandleTypeDef htim3;

//variables externas de la UART
extern  uint8_t uart1ReceivedData;
extern UART_HandleTypeDef huart1;
//extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern volatile uint8_t inputBuffer[64];
extern volatile uint8_t inputIndex;
extern volatile uint8_t uart_rx_complete;

typedef enum {				//enum para elegir el dispositivo de salida de los datos
	UART1 = 0,
	UART2,
	UART3,
	USB_SERIAL
} serialDevice_t;

extern serialDevice_t serialDevice;		//variable serialDevice inicializada en main.
										//acepta los valores definidos en el enum (UART1, USB_SERIAL)


//variables externas para el adc
extern volatile uint8_t adcConverted;
extern ADC_HandleTypeDef hadc1;


//compensador
typedef struct 			//estructura que guarda los coeficientes del compensador digital
{
	float denCoef[3];				//denominador
	float numCoef[3];				//numerador
	float intGain;					//ganancia integrador
} comp_t;

extern comp_t digitalComp;

#define ADC_MAX_SAMPLES 50			//cantidad maxima de muestras que debe tomar el adc
									//hay que tener en cuenta la cantidad de canales del adc que se leen
extern const uint32_t ADC_SAMPLE_FREQ;		//frecuencia de muestreo del adc
extern volatile uint16_t adcBuf[ADC_MAX_SAMPLES];			//buffer para almacenar las muestras del ADC

#endif /* INC_COMMON_VARIABLES_H_ */
