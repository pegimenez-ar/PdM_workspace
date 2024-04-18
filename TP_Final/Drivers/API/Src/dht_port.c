/**
 ********************************************************************************
 * @file    dht_port.c
 * @author  Ing. Pablo E. Gimenez - pegimenez@fi.uba.ar
 * @date    Apr 14, 2024
 * @brief   La libreria dht_port implementa un las funciones correspondientes al
 * 			hardware utilizado en la placa seleccionada. Se desarrolla como trabajo
 * 			final para las materias de Programacion de Microcontroles y Protocolos
 * 			de Comunicacion de Microcontroladores del CESE_222024
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "dht_port.h"

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

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
/**
 * @brief  Funcion que setea el pin del GPIO en Modo Output.
 * @param  Puntero a sensor para tomar luego Puerto GPIO &
 * 			Pin GPIO
 * @retval None
 */
void DHT_PORT_SetPinOutput(dht_t * sensor)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = sensor->GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(sensor->GPIO_Port, &GPIO_InitStruct);
}

/**
 * @brief  Funcion que setea el pin del GPIO en Modo Input.
 * @param  Puntero a sensor para tomar luego Puerto GPIO &
 * 			Pin GPIO
 * @retval None
 */
void DHT_PORT_SetPinInput(dht_t * sensor)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = sensor->GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(sensor->GPIO_Port, &GPIO_InitStruct);
}

/**
 * @brief  Funcion setea el estado del pin del GPIO .
 * @param  Puntero a sensor para tomar luego Puerto GPIO &
 * 			Pin GPIO y estado a setear (LOW / HIGH)
 * @retval None
 */
void DHT_PORT_SetPinState(dht_t * sensor, dhtPortState_t state)
{

	HAL_GPIO_WritePin(sensor->GPIO_Port,sensor->GPIO_Pin, (GPIO_PinState) state);
}

/**
 * @brief  Funcion setea el estado del pin del GPIO .
 * @param  Puntero a sensor para tomar luego Puerto GPIO &
 * 			Pin GPIO y estado a setear (LOW / HIGH)
 * @retval None
 */
uint32_t DHT_PORT_ExpectPulse(dht_t * sensor, dhtPortState_t state)
{
	uint32_t _count = 0;
	const uint32_t _maxcycles = DHT_PORT_TIMEOUT * (HAL_RCC_GetHCLKFreq() / 1000000);

	while (HAL_GPIO_ReadPin(sensor->GPIO_Port,sensor->GPIO_Pin)== (GPIO_PinState) state){
		if (_count++ >= _maxcycles){
			_count = COUNT_TIMEOUT;
			return _count;
		}
	}

	return _count;
}
