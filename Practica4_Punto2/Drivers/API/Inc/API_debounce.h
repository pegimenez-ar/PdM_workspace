 /**
  ******************************************************************************
  * @file    API_debounce.h
  * @author  Pablo Gimenez
  * @brief   Archivo que contiene las declaraciones de la MEF
  ******************************************************************************
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdbool.h>
#include "API_delay.h"
#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */

/* Defines -------------------------------------------------------------------*/
#define DELAY_BOUNCE 40		//valor en milisegundos para el manejo del antibounce

/*Definicion de tipos*/
typedef bool bool_t;

/*Definicion de funciones*/
void debounceFSM_init(void);
void debounceFSM_update(void);
bool_t readKey(void);

#endif /* API_INC_API_DEBOUNCE_H_ */
