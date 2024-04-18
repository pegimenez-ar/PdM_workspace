/**
 ********************************************************************************
 * @file    API_delay.h
 * @author  Ing. Pablo E. Gimenez - pegimenez@fi.uba.ar
 * @date    Apr 14, 2024
 * @brief   La libreria API_delay implementa delays no bloqueantes con funciones
 * 			de la HAL como trabajo final para las materias de Programacion de
 * 			Microcontroles y Protocolos de Comunicacion de Microcontroladores
 * 			del CESE_222024
 ********************************************************************************
 */

#ifndef API_INC_API_DELAY_H_
#define API_INC_API_DELAY_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */

/************************************
 * MACROS AND DEFINES
 ************************************/

/************************************
 * TYPEDEFS
 ************************************/
typedef uint32_t tick_t;
typedef bool bool_t;

typedef struct  {
	tick_t startTime;
	tick_t duration;
	bool_t running;
} delay_t;

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void delayInit (delay_t* delay, tick_t duration);
bool_t delayRead (delay_t* delay);
void delayWrite (delay_t* delay, tick_t duration);


#ifdef __cplusplus
}
#endif

#endif /* API_INC_API_DELAY_H_ */
