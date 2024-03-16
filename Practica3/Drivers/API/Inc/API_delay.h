 /**
  ******************************************************************************
  * @file    main.c
  * @author  Pablo Gimenez
  * @brief   Ejercicio que alterna distintos leds con retarnos no bloqueantes
  ******************************************************************************
 */

#ifndef API_INC_API_DELAY_H_
#define API_INC_API_DELAY_H_

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */

/*Definicion de tipos*/
typedef uint32_t tick_t;
typedef bool bool_t;

typedef struct  {
	tick_t startTime;
	tick_t duration;
	bool_t running;
} delay_t;

/*Definicion de funciones*/
void delayInit (delay_t* delay, tick_t duration);

bool_t delayRead (delay_t* delay);

void delayWrite (delay_t* delay, tick_t duration);


#endif /* API_INC_API_DELAY_H_ */
