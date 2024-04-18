/**
 ********************************************************************************
 * @file    dht_port.h
 * @author  Ing. Pablo E. Gimenez - pegimenez@fi.uba.ar
 * @date    Apr 14, 2024
 * @brief   La libreria dht_port implementa un las funciones correspondientes al
 * 			hardware utilizado en la placa seleccionada. Se desarrolla como trabajo
 * 			final para las materias de Programacion de Microcontroles y Protocolos
 * 			de Comunicacion de Microcontroladores del CESE_222024
 ********************************************************************************
 */
#ifndef API_INC_DHT_PORT_H_
#define API_INC_DHT_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif


/************************************
 * INCLUDES
 ************************************/
#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "dht.h"

/************************************
 * MACROS AND DEFINES
 ************************************/
#define DHT_PORT_TIMEOUT 1000U
#define COUNT_TIMEOUT 0xFFFFFFFF

/************************************
 * TYPEDEFS
 ************************************/
typedef enum
{
	LOW = 0,
	HIGH
}dhtPortState_t;

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void DHT_PORT_SetPinOutput(dht_t * sensor);
void DHT_PORT_SetPinInput(dht_t * sensor);
void DHT_PORT_SetPinState(dht_t * sensor, dhtPortState_t state);
uint32_t DHT_PORT_ExpectPulse(dht_t * sensor, dhtPortState_t state);

#ifdef __cplusplus
}
#endif

#endif /* API_INC_DHT_PORT_H_ */
