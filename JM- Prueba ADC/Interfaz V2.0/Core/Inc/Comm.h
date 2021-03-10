#ifndef INC_COM_H
#define INC_COMM_H_

#include "main.h"



typedef enum
{
	CMD_NULL = 0,
	INICIO,
	DETENER,
	ETC
}comando_nombre;


typedef struct
{
	comando_nombre name;
	float coeficientes[7];
}comando_in;

comando_in comm_parse(uint8_t *data_buff);
void comm_case(comando_in comando_uart);



#endif /* INC_COMM_H_ */
