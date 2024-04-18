/**
 ********************************************************************************
 * @file    API_debounce.h
 * @author  Ing. Pablo E. Gimenez - pegimenez@fi.uba.ar
 * @date    Apr 14, 2024
 * @brief   La libreria API_debounce implementa un antirebote para el boton de
 * 			usuario de la placa nucleo-144. Se desarrollo como trabajo final
 * 			para las materias de Programacion de Microcontroles y Protocolos
 * 			de Comunicacion de Microcontroladores
 * 			del CESE_222024
 ********************************************************************************
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

#ifdef __cplusplus
extern "C" {
#endif


/************************************
 * INCLUDES
 ************************************/
#include <stdio.h>
#include <stdbool.h>
#include "API_delay.h"
#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */

/************************************
 * MACROS AND DEFINES
 ************************************/
#define DELAY_BOUNCE 40		//valor en milisegundos para el manejo del antibounce

/************************************
 * TYPEDEFS
 ************************************/
typedef bool bool_t;

/*Definicion de funciones*/
void debounceFSM_init(void);
void debounceFSM_update(void);
bool_t readKey(void);

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/

#ifdef __cplusplus
}
#endif

#endif /* API_INC_API_DEBOUNCE_H_ */
