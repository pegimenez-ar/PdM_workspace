/**
 ********************************************************************************
 * @file    dht.h
 * @author  Ing. Pablo E. Gimenez - pegimenez@fi.uba.ar
 * @date    Apr 14, 2024
 * @brief   La libreria dht implementa un driver para sensores DHT basados en
 * 			protocolo de comunicacion One Wire. Se desarrolla como trabajo final
 * 			para las materias de Programacion de Microcontroles y Protocolos
 * 			de Comunicacion de Microcontroladores
 * 			del CESE_222024
 ********************************************************************************
 */

#ifndef API_INC_DHT_H_
#define API_INC_DHT_H_

#ifdef __cplusplus
extern "C" {
#endif


/************************************
 * INCLUDES
 ************************************/
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"  		/* <- HAL include */

/************************************
 * MACROS AND DEFINES
 ************************************/
#define STM32F4xx
//#define OTR

/************************************
 * TYPEDEFS
 ************************************/
typedef enum
{
  DHT_OK       		= 0x00U,
  DHT_ERROR    		= 0x01U,
  DHT_TIMEOUT  		= 0x02U,
  DHT_CHECKSUM		= 0x03U
} dhtStatusType_t;

typedef enum
{
	DHT_VALID_DATA,
	DHT_NO_VALID_DATA
} dhtDataValid_t;

typedef enum
{
	DHT11,
	DHT22,
	AM2301
} dhtType_t;

typedef enum
{
	C = 0x01U,
	F = 0x02U
} tempUnit_t;

typedef struct
{
#ifdef STM32F4xx
	GPIO_TypeDef	*GPIO_Port;
	uint8_t			GPIO_Pin;
#endif
	dhtType_t		type;
	float			temperature;
	float			humidity;
	tempUnit_t 		temperatureUnit;
	uint32_t		lastRead;
	dhtDataValid_t	dataValid;
} dht_t;

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
#ifdef STM32F4xx
dhtStatusType_t DHT_Init(dht_t* sensor, GPIO_TypeDef* GPIO_Port, uint8_t GPIO_Pin, dhtType_t type);
#endif
dhtStatusType_t DHT_Read(dht_t* sensor, tempUnit_t unit);
dhtStatusType_t DHT_ConvertUnit(dht_t* sensor, tempUnit_t unit);


#ifdef __cplusplus
}
#endif

#endif /* API_INC_DHT_H_ */
