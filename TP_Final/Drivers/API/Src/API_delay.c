/**
 ********************************************************************************
 * @file    API_delay.c
 * @author  Ing. Pablo E. Gimenez - pegimenez@fi.uba.ar
 * @date    Apr 14, 2024
 * @brief   La libreria API_delay implementa delays no bloqueantes con funciones
 * 			de la HAL como trabajo final para las materias de Programacion de
 * 			Microcontroles y Protocolos de Comunicacion de Microcontroladores
 * 			del CESE_222024
 ********************************************************************************
 */
/************************************
 * INCLUDES
 ************************************/
#include "API_delay.h"

/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/

/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/
static void API_DELAY_Error_Handler(void);

/************************************
 * STATIC FUNCTIONS
 ************************************/

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
/**
  * @brief  Inicializacion de las variables de delay
  * @param  Se debe entregar el puntero a la variable y la duración expresado en milisegundos
  * @retval Ninguno
  * @error Se validan que el puntero entregado no sea nulo y que la duracion sea distinta de cero
*/
void delayInit (delay_t* delay, tick_t duration) {

	if (delay == NULL	|| duration == 0)
		API_DELAY_Error_Handler();

	*delay = (delay_t) {
		.startTime = 0,
		.duration =  duration,
		.running = false
	};

}

/**
  * @brief  Lectura del estado del delay - Cumplido o no Cumplido
  * @param  Se debe entregar el puntero a la variable a evaluar
  * @retval Retorna false en caso qeu la variable no esta inicia y la inicia
  * 		o si no se ha cumplido el tiempo definido
  * 		Retorna true cuando cuando se ha cumplido el tiempo definido
  * @error Se validan que el puntero entregado no sea nulo
*/


bool_t delayRead (delay_t* delay) {

	if (delay == NULL)
		API_DELAY_Error_Handler();

	bool_t timeOut = false;

	uint32_t currentTime = HAL_GetTick();


	if (!(delay->running)) {

		delay->running = true;

		delay->startTime = currentTime;

	} else {

		if ((currentTime - delay->startTime) >= delay->duration) {

			timeOut = true;

			delay->running = false;

		}

	}

	return timeOut;
}

/**
  * @brief  Cambio del valor de delay  de una variable
  * @param  Se debe entregar el puntero a la variable y el nuevo valor a modificar. Valor expresado en milisegundos
  * @retval Nada
  * @error Se validan que el puntero entregado no sea nulo
  *
*/
void delayWrite (delay_t* delay, tick_t duration) {

	if (delay == NULL)
		API_DELAY_Error_Handler();

	if (duration >= 0) {

		delay->duration = duration;

	}

}

static void API_DELAY_Error_Handler(void)
{
	/* Turn LED2 on */
	BSP_LED_On(LED1);
	while (1)
	{
	}
}

