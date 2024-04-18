/**
 ********************************************************************************
 * @file    dht.c
 * @author  Ing. Pablo E. Gimenez - pegimenez@fi.uba.ar
 * @date    Apr 14, 2024
 * @brief   La libreria dht implementa un driver para sensores DHT basados en
 * 			protocolo de comunicacion One Wire. Se desarrolla como trabajo final
 * 			para las materias de Programacion de Microcontroles y Protocolos
 * 			de Comunicacion de Microcontroladores
 * 			del CESE_222024
 ********************************************************************************
 */
/************************************
 * INCLUDES
 ************************************/
#include "dht.h"
#include "dht_port.h"
#include "stm32f4xx_nucleo_144.h"

/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define MIN_INTERVAL_DHT 2000
#define DHT_TIMEOUT_COUNT 0xFFFFFFFF
#define LOW_STATE 0
#define HIGH_STATE 1

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
static inline void _DHT_delay(volatile uint32_t microseconds);
static uint32_t _DWT_Delay_Init(void);
static dhtStatusType_t _DHT_GetData(dht_t* sensor);
static void _DHT_ComputeData(dht_t* sensor, uint8_t data[]);

/************************************
 * STATIC FUNCTIONS
 ************************************/

/**
 * @brief Esta funcion recolecta la data del sensor
 * @param  Puntero al handler del sensor
 * @retval Una variable de estado segun se realizo la operacion
 */
static dhtStatusType_t _DHT_GetData(dht_t* sensor)
{
	dhtStatusType_t _sensor_status = DHT_OK;
	uint8_t data[5] = {0};

	//Inicializamos la funcion delay
	_DWT_Delay_Init();

	/*	Seteamos el pin como output, luego seteamos el estado bajo, se espera
	 * 	un delay segun tipo sensor y luego en estado alto para luego setear el
	 * 	pin como input y recibir respuesta del sensor
	 */
	DHT_PORT_SetPinOutput(sensor);

	DHT_PORT_SetPinState(sensor, LOW);

	switch(sensor->type){
	case DHT22:
	case AM2301:
		_DHT_delay(1200);
		break;

	case DHT11:
	default:
		_DHT_delay(18000);
		break;
	}

    DHT_PORT_SetPinState(sensor, HIGH);

    _DHT_delay(30);

	DHT_PORT_SetPinInput(sensor);

	/*	Esperamos la respueta del sensor. Son 80 us estado bajo y 80 us
	 * 	estado alto. Se toma una lectura en 40 us y luego a los 80 us lo
	 * 	cual corresponde en la mitad de cada pulso bajo y alto que se esperan
	 */

	// First expect a low signal for ~80 microseconds followed by a high signal
	// for ~80 microseconds again.
	_DHT_delay(5);
	if(DHT_PORT_ExpectPulse(sensor,LOW) == DHT_TIMEOUT_COUNT) {
		_sensor_status = DHT_TIMEOUT;
		return _sensor_status;
	}
	_DHT_delay(5);
	if(DHT_PORT_ExpectPulse(sensor,HIGH) == DHT_TIMEOUT_COUNT) {
		_sensor_status = DHT_TIMEOUT;
		return _sensor_status;
	}

	uint32_t _cycles[80];

 	for(int i = 0; i < 80; i += 2) {
 		_cycles[i] = DHT_PORT_ExpectPulse(sensor,LOW);
		_cycles[i+1] = DHT_PORT_ExpectPulse(sensor,HIGH);
	}

	for(int i = 0; i < 40; ++i) {
		uint32_t _lowCycles = _cycles[2 * i];
		uint32_t _highCycles = _cycles[2 * i + 1];
		if((_lowCycles == DHT_TIMEOUT) || (_highCycles == DHT_TIMEOUT)) {
			_sensor_status = DHT_ERROR;
			return _sensor_status;
		}
		data[i / 8] <<= 1;

		if(_highCycles > _lowCycles) {

			data[i / 8] |= 1;
		}

	}

	// Se validad que los checksum correspondan
	if (data[4] != ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
		_sensor_status = DHT_CHECKSUM;
		return _sensor_status;
	}

	_DHT_ComputeData(sensor, data);

	return DHT_OK;
}

/**
 * @brief Esta funcion procesa la informacion recolectada del sensor
 * @param  Puntero al handler del sensor
 * @param  Un array con la informacion recolectada
 * @retval No devuelve nada por nombre, almacene en puntero
 */
static void _DHT_ComputeData(dht_t* sensor, uint8_t data[])
{
	float value = 0;

	if(data == NULL){
		sensor->dataValid = DHT_NO_VALID_DATA;
		return;
	}

	switch(sensor->type){
	case DHT11:
		//Procesa Temperatura
		value = data[2];
		sensor->temperature = value;
		//Procesa Humedad
		value = data[0];
		sensor->humidity = value;

		break;

	case DHT22:
	case AM2301:
		//Procesa Temperatura
		value = (data[2] & 0x7F) << 8 | data[3];
		value *= 0.1;
		if (data[2] & 0x80) {
			value *= -1;
		}
		sensor->temperature = value;

		//Procesa Humedad
		value = data[0] << 8 | data[1];
		value *= 0.1;
		sensor->humidity = value;

		break;

	default:
		//En caso que se llegue a esta opcion hubo error y se coloca data como invalida
		sensor->dataValid = DHT_NO_VALID_DATA;
		return;
		break;
	}

	sensor->dataValid = DHT_VALID_DATA;

	return;
}



/**
 * @brief Esta funcion inicializa el Delay en microsegundo
 * @param  None
 * @retval Correcto o no correcto
 */
static uint32_t _DWT_Delay_Init(void)
{
  /* Disable TRC */
  CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
  /* Enable TRC */
  CoreDebug->DEMCR |=  CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;

  /* Disable clock cycle counter */
  DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
  /* Enable  clock cycle counter */
  DWT->CTRL |=  DWT_CTRL_CYCCNTENA_Msk; //0x00000001;

  /* Reset the clock cycle counter value */
  DWT->CYCCNT = 0;

     /* 3 NO OPERATION instructions */
     __ASM volatile ("NOP");
     __ASM volatile ("NOP");
  __ASM volatile ("NOP");

  /* Check if clock cycle counter has started */
     if(DWT->CYCCNT)
     {
       return 0; /*clock cycle counter started*/
     }
     else
  {
    return 1; /*clock cycle counter not started*/
  }
}

/**
 * @brief Esta funcion implementa Delay bloqueante en microsegundo
 * @param  MIcrosegundo de delay
 * @retval None
 */
static inline void _DHT_delay(volatile uint32_t microseconds)
{
  uint32_t clk_cycle_start = DWT->CYCCNT;

  /* Go to number of cycles for system */
  microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);

  /* Delay till end */
  while ((DWT->CYCCNT - clk_cycle_start) < microseconds);
}


/************************************
 * GLOBAL FUNCTIONS
 ************************************/
/**
 * @brief Esta funcion inicializa el sensor
 * @param  Puntero al handler del sensor
 * @param  Puntero al handler del puerto GPIO
 * @param  Pin del puerto GPIO
 * @param  Tipo de sensor DHT11 o DHT22
 * @retval Una variable de estado del proceso
 */
dhtStatusType_t DHT_Init(dht_t* sensor, GPIO_TypeDef* GPIO_Port, uint8_t GPIO_Pin, dhtType_t type)
{
	sensor->GPIO_Port = GPIO_Port;
	sensor->GPIO_Pin = GPIO_Pin;
	sensor->type = type;
	sensor->temperature = 0;
	sensor->humidity = 0;
	sensor->temperatureUnit = C;							//Unidad °C por default
	sensor->lastRead = HAL_GetTick() - MIN_INTERVAL_DHT;	//Inicializamos con valor pasado
	sensor->dataValid = DHT_NO_VALID_DATA;

	return DHT_OK;
}

/**
 * @brief Esta funcion realiza la lectura del sensor
 * @param  Puntero al handler del sensor
 * @param  Unidad enq ue se desea la medicion
 * @retval Una variable de estado del proceso
 */
dhtStatusType_t DHT_Read(dht_t* sensor, tempUnit_t unit)
{
	dhtStatusType_t status = DHT_OK;

	uint32_t currentTime = HAL_GetTick();

	if( (sensor->lastRead - currentTime) < MIN_INTERVAL_DHT)
		return status;

	sensor->lastRead = currentTime;

	status = _DHT_GetData(sensor);

	if( status != DHT_OK)
		return status;

	if( unit == F)
		status = DHT_ConvertUnit(sensor, F);

	return status;
}

/**
 * @brief Esta funcion realiza la conversion de unidades
 * @param  Puntero al handler del sensor
 * @param  Unidad enq ue se desea la medicion
 * @retval Una variable de estado del proceso
 */
dhtStatusType_t DHT_ConvertUnit(dht_t* sensor, tempUnit_t unit)
{
	dhtStatusType_t status = DHT_OK;

	if(sensor->dataValid == DHT_NO_VALID_DATA){
		status = DHT_ERROR;
		return status;
	}

	if(sensor->temperatureUnit == unit){
		status = DHT_OK;
		return status;
	}

	float temperature = sensor->temperature;

	switch(unit){
	case C:
		temperature = temperature * 1.8 + 32;
		break;

	case F:
	default:

		temperature = temperature * 1.8 + 32;
		break;
	}

	sensor->temperature = temperature;
	sensor->temperatureUnit = unit;

	return status;
}
