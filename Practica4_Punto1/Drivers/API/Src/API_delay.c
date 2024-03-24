 /**
  ******************************************************************************
  * @file    main.c
  * @author  Pablo Gimenez
  * @brief   Ejercicio que alterna distintos leds con retarnos no bloqueantes
  ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "API_delay.h"

/* Private Functions Declaration ---------------------------------------------*/
static void API_DELAY_Error_Handler(void);

/* Desarrollo de las funciones solicitadas */

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
	BSP_LED_On(LED2);
	while (1)
	{
	}
}

