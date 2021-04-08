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

typedef struct
{
	int sign_in;
	int sign_feed;
} sumador_t;



//compensador
typedef struct 			//estructura que guarda los coeficientes y señales del compensador digital
{
	float Yref;			//posición de referencia en m
	float Yestimada;	//posición estimada en m

	sumador_t sum1;
	sumador_t sum2;
	float Kint;					//ganancia del integrador
	float Ts;					//periodo de muestreo
	float error1_n;				//error en n
	float error1_n1;			//error en n-1
	float control1_n;				//salida del integrador en n
	float control1_n1;			//salida del integrador en n-1
	float error2_n;				//error del segundo sumador en n
	float error2_n1;
	float error2_n2;
	float control2_n;				//salida del compensador por adelanto de fase en n
	float control2_n1;
	float control2_n2;

	float denCoef[3];				//coeficientes que afectan a las salidas anteriores del compensador
	float numCoef[3];				//coeficientes que afectan a la entrada actual y pasadas

} comp_t;

extern comp_t digitalComp;

#define ADC_MAX_SAMPLES 50 * 2			//cantidad maxima de muestras que debe tomar el adc. multiplicado por la cantidad de canales
									//hay que tener en cuenta la cantidad de canales del adc que se leen
extern const uint32_t ADC_SAMPLE_FREQ;		//frecuencia de muestreo del adc
extern volatile uint16_t adcBuf[ADC_MAX_SAMPLES];			//buffer para almacenar las muestras del ADC

#endif /* INC_COMMON_VARIABLES_H_ */
