 /**
  ******************************************************************************
  * @file    main.c
  * @author  Pablo Gimenez
  * @brief   Ejercicio que alterna distintos leds con retarnos no bloqueantes
  ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "API_delay.h"

/* Desarrollo de las funciones solicitadas */

/**
  * @brief  Inicializacion de las variables de delay
  * @param  Se debe entregar el puntero a la variable y la duración expresado en milisegundos
  * @retval Ninguno
*/
void delayInit (delay_t* delay, tick_t duration) {

	if (delay == NULL	|| duration == 0)
		Error_Handler();

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
  *
*/

bool_t delayRead (delay_t* delay) {

	bool_t timeOut = false;

	uint32_t currentTime = HAL_GetTick();

	if (delay == NULL)
		Error_Handler();

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
  *
*/
void delayWrite (delay_t* delay, tick_t duration) {

	if (delay == NULL)
		Error_Handler();

	if (duration >= 0) {

		delay->duration = duration;

	}

}
