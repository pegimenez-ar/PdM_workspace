/**
 ********************************************************************************
 * @file    API_lcd_port.h
 * @author  Ing. Pablo E. Gimenez - pegimenez@fi.uba.ar
 * @date    Apr 14, 2024
 * @brief   La libreria API_lcd_port implementa el manejo de las funciones de la
 * 			HAL corrrespondiente a la arquitectura de hardware de STM32F4
 * 			como trabajo final para las materias de Programacion de
 * 			Microcontroles y Protocolos de Comunicacion de Microcontroladores
 * 			del CESE_222024
 ********************************************************************************
 */

#ifndef API_INC_API_LCD_PORT_H_
#define API_INC_API_LCD_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "API_lcd.h"

/************************************
 * MACROS AND DEFINES
 ************************************/

/************************************
 * TYPEDEFS
 ************************************/

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
bool API_LCD_PORT_Init(lcdType_t *lcd);
bool API_LCD_PORT_Transmit(lcdType_t *lcd, uint8_t value);



#ifdef __cplusplus
}
#endif

#endif 
