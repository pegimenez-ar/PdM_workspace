/**
 ********************************************************************************
 * @file    API_FSM.c
 * @author  Ing. Pablo E. Gimenez - pegimenez@fi.uba.ar
 * @date    Apr 14, 2024
 * @brief   La libreria API_FSM implementa la maquina de estados finitos
 * 			planteada para resolver la propuesta de trabajo final para las
 * 			materias de Programacion de Microcontroles y Protocolos de
 * 			Comunicacion de Microcontroladores del CESE_222024
 ********************************************************************************
 */

#ifndef API_INC_API_FSM_H_
#define API_INC_API_FSM_H_

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
#include "dht.h"
#include "API_debounce.h"
#include "API_lcd.h"
#include "main.h"

/************************************
 * MACROS AND DEFINES
 ************************************/
#define LCD_TIMER_OFF					30000		//Valor expresado en mseg

/************************************
 * TYPEDEFS
 ************************************/
typedef bool bool_t;

/************************************
 * EXPORTED VARIABLES
 ************************************/
extern dht_t sensorTemp;
extern lcdType_t lcd;
extern I2C_HandleTypeDef hi2c1;

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void API_FSM_init(void);
void API_FSM_update(void);
tempUnit_t API_FSM_tempUnit(void);

#ifdef __cplusplus
}
#endif

#endif /* API_INC_API_FSM_H_ */
