/**
 ********************************************************************************
 * @file    API_lcd_port.c
 * @author  Ing. Pablo E. Gimenez - pegimenez@fi.uba.ar
 * @date    Apr 14, 2024
 * @brief   La libreria API_lcd_port implementa el manejo de las funciones de la
 * 			HAL corrrespondiente a la arquitectura de hardware de STM32F4
 * 			como trabajo final para las materias de Programacion de
 * 			Microcontroles y Protocolos de Comunicacion de Microcontroladores
 * 			del CESE_222024
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "API_lcd_port.h"
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */
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

/************************************
 * STATIC FUNCTIONS
 ************************************/
/**
 * @brief  Funcion diseña para container de error
 * @param  None
 * @retval None
 */
static void API_LCD_PORT_Error_Handler(void)
{
	/* Turn LED2 on */
	BSP_LED_On(LED2);
	while (1)
	{
	}
}

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
/**
 * @brief  Esta función inicializa el puerto I2C. Se debio configurar MSP
 * 			y GPIO previamente.
 * @param  Puntero al handler del LCD
 * @retval Un valor false si fue exitoso y true si hubo error
 */
bool API_LCD_PORT_Init(lcdType_t *lcd)
{
	if(lcd == NULL)
		API_LCD_PORT_Error_Handler();

	if(HAL_I2C_Init (lcd->hi2c1) != HAL_OK)
		return true;
	else
		return false;

}

/**
 * @brief  Esta función envia datos por el puerto I2C. Se debio iniciarlizar previamente
 * 			el puerto I2C
 * @param  Puntero al handler del LCD
 * @retval Un valor false si fue exitoso y true si hubo error
 */
bool API_LCD_PORT_Transmit(lcdType_t *lcd, uint8_t value){

	if(lcd == NULL)
		API_LCD_PORT_Error_Handler();

	//Se hace la transmisión y en base al retorno (HAL_OK o HAL_NOK) se retorna true o false

	if(HAL_I2C_Master_Transmit(lcd->hi2c1, (lcd->address)<<1 ,&value, sizeof(value),HAL_MAX_DELAY)!=HAL_OK)
		return true;
	else
		return false;

}
