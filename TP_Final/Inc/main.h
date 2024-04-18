/**
 ********************************************************************************
 * @file    main.h
 * @author  Ing. Pablo E. Gimenez - pegimenez@fi.uba.ar
 * @date    Apr 14, 2024
 * @brief   Trabajo final de las materias Programacion de Microcontroles y
 * 			Protocolos de Comunicacion de Microcontroladores del CESE_222024
 * 			Se ha implementado un dispositivo que toma la temperatura de un sensor
 * 			dht y lo muesta en pantalla de un lcd HD44780
 ********************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */
#include "stm32f4xx_hal_i2c.h"
#include "dht.h"
#include "API_lcd.h"
#include "API_delay.h"
#include "API_FSM.h"



/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor USARTx/UARTx instance used and associated
   resources */
/* Definition for USARTx clock resources */
#define USARTx                           USART3
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART3_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART3_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_8
#define USARTx_TX_GPIO_PORT              GPIOD
#define USARTx_TX_AF                     GPIO_AF7_USART3
#define USARTx_RX_PIN                    GPIO_PIN_9
#define USARTx_RX_GPIO_PORT              GPIOD
#define USARTx_RX_AF                     GPIO_AF7_USART3

//Definiciones para sensor temperatura
#define DHT_SENSOR_TYPE					DHT22
#define DHT_SENSOR_GPIO_PORT			GPIOG
#define DHT_SENSOR_GPIO_PIN				GPIO_PIN_2
#define DHT_SENSOR_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOG_CLK_ENABLE()
#define DHT_SAMPLE_PERIOD				60000	//valor expresado en mseg.

//Definiciones para LCD
#define LCD_I2C_ADDRESS					0x27
#define LCD_COLUMNS						20U
#define LCD_ROWS						4U
#define LCD_CHARACTER					LCD_5x8DOTS
#define LCD_BITMODE						LCD_4BITMODE


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
