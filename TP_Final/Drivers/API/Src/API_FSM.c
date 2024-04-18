/**
 ********************************************************************************
 * @file    API_FSM.c
 * @author  Ing. Pablo E. Gimenez - pegimenez@fi.uba.ar
 * @date    Apr 14, 2024
 * @brief   La libreria API_delay implementa delays no bloqueantes con funciones
 * 			de la HAL como trabajo final para las materias de Programacion de
 * 			Microcontroles y Protocolos de Comunicacion de Microcontroladores
 * 			del CESE_222024
 ********************************************************************************
 */
/************************************
 * INCLUDES
 ************************************/
#include "API_FSM.h"
#include "math.h"

/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
*************************************/

/************************************
 * PRIVATE TYPEDEFS
 ************************************/
typedef enum{
	BACKLIGHT_ON_C,
	BACKLIGHT_OFF_C,
	BACKLIGHT_ON_F,
	BACKLIGHT_OFF_F
} fsmState_t;

/************************************
 * STATIC VARIABLES
 ************************************/

const uint8_t temp_Text[]= "Temp: ";
const uint8_t hum_Text[]= "Hum: ";
const uint8_t CUnit_Text[]= " C";
const uint8_t FUnit_Text[]= " F";
const uint8_t PorUnit_Text[]= " %";
static fsmState_t estadoMEF;
static delay_t delayLCD, *p_delayLCD, delayTemp, *p_delayTemp;

/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/
static void _API_FSM_turnOffBacklight(void);
static void _API_FSM_turnOnBacklight(void);
static void _API_FSM_Error_Handler(void);
static void _API_FSM_Process_Temperature_Init(void);
static void _API_FSM_Process_Temperature(void);
static void _API_FSM_UpdateLCD(void);
static void reverse(char* str, int len);
static int intToStr(int x, char str[], int d);
static void ftoa(float n, char* res, int afterpoint);

/************************************
 * STATIC FUNCTIONS
 ************************************/

/**
  * @brief  Funcion que inicializa la Maquina de Estados Finitos con un estado definido
  * 		por default
  * @param  None
  * @retval None
  */
void API_FSM_init(void){

	/*Se inicializa la maquina de estado a su valor por defecto, los timers, el debounce
	 * del pulsador de usuario, el LCD, el sensor de temperatura y su proceso.
	 */
	estadoMEF = BACKLIGHT_ON_C;

	p_delayLCD = &delayLCD;

	delayInit( p_delayLCD , LCD_TIMER_OFF);

	p_delayTemp = &delayTemp;

	delayInit( p_delayTemp, DHT_SAMPLE_PERIOD);

	debounceFSM_init();

	//Se inicaliza el LCD
	if(lcdInit(&lcd, &hi2c1, LCD_I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS, LCD_CHARACTER, LCD_BITMODE) != LCD_OK)
		_API_FSM_Error_Handler();

	_API_FSM_Process_Temperature_Init();

	//Se toma una medición inicial de temperatura como partida
	DHT_Read(&sensorTemp, C);

	//Se actualiza los datos en el LCD
	_API_FSM_UpdateLCD();

	return;

}

/**
  * @brief  Funcion que actualiza el estado de la Maquina de Estados Finitos según haya cambios en la
  * 		entrada (pulsador presionado) o en base al tiempo transcurrido desde el cambio de estado
  * 		de la entrada en las anteriores ejecuciones
  * @param  None
  * @retval None
  */
void API_FSM_update(void){

	debounceFSM_update();

	_API_FSM_Process_Temperature();

	switch(estadoMEF){

	case BACKLIGHT_ON_C:
		if(readKey()){
			DHT_ConvertUnit(&sensorTemp, F);
			_API_FSM_UpdateLCD();
			estadoMEF = BACKLIGHT_ON_F;
		}
		else if(delayRead(p_delayLCD)){
			 _API_FSM_turnOffBacklight();
			estadoMEF = BACKLIGHT_OFF_C;
		}
		break;

	case BACKLIGHT_OFF_C:
		if(readKey()){
			_API_FSM_turnOnBacklight();
			estadoMEF = BACKLIGHT_ON_C;
		}
		break;

	case BACKLIGHT_ON_F:
		if(readKey()){
			DHT_ConvertUnit(&sensorTemp, C);
			_API_FSM_UpdateLCD();
			estadoMEF = BACKLIGHT_ON_C;
		}
		else if(delayRead(p_delayLCD)){
			 _API_FSM_turnOffBacklight();
			estadoMEF = BACKLIGHT_OFF_F;
		}
		break;

	case BACKLIGHT_OFF_F:
		if(readKey()){
			_API_FSM_turnOnBacklight();
			estadoMEF = BACKLIGHT_ON_F;
		}
		break;

	/*CASO DEFAULT: En caso de no haber ejecutado una accion por los posibles
	 * estados anteriornes indica que la maquina de estado no se encuentra en
	 * un estado posible o válido por algun motivo por lo cual se llama a la
	 * funcion error
	 */
	default:
		_API_FSM_Error_Handler();
		break;
	}

}
/*
tempUnit_t API_FSM_tempUnit(void)
{
	switch(estadoMEF){

	case BACKLIGHT_ON_C:
	case BACKLIGHT_OFF_C:
	default:
		return C;
		break;

	case BACKLIGHT_ON_F:
	case BACKLIGHT_OFF_F:

		return F;
		break;
	}
}
*/
/**
  * @brief  Funcion que ejecuta accion de apagar el backlight)
  * @param  None
  * @retval None
  */
static void _API_FSM_turnOffBacklight(void)
{
	lcdNoBacklight(&lcd);

	return;
}

/**
  * @brief  Funcion que ejecuta accion de encender el backlight)
  * @param  None
  * @retval None
  */
static void _API_FSM_turnOnBacklight(void)
{
	lcdBacklight(&lcd);

	return;
}

static void _API_FSM_Process_Temperature_Init(void)
{
	//Aseguramos que esta el clock del bus encendido
	DHT_SENSOR_GPIO_CLK_ENABLE();

	//Inicializamos el sensor de temperatura
	if(DHT_Init(&sensorTemp, DHT_SENSOR_GPIO_PORT, DHT_SENSOR_GPIO_PIN, DHT_SENSOR_TYPE)!=DHT_OK)
		_API_FSM_Error_Handler();

	//Inicializamos el delay para la toma de temperatura
	delayInit (&delayTemp, DHT_SAMPLE_PERIOD);

	return;
}

static void _API_FSM_Process_Temperature(void)
{
	if(delayRead (&delayTemp)){
		tempUnit_t actualUnit = sensorTemp.temperatureUnit;
		DHT_Read(&sensorTemp, actualUnit);
		_API_FSM_UpdateLCD();
	}

	return;

}

/**
  * @brief  Funcion destinada a contenedor de manejo de errores
  * @param  None
  * @retval None
  */
static void _API_FSM_UpdateLCD(void)
{
	float temp = sensorTemp.temperature;
	char temp_Value[5] = {0};
	char temp_Value2[5] = {0};

	lcdClear(&lcd);
	lcdReturnHome(&lcd);

	lcdPrintStr(&lcd,temp_Text, sizeof(temp_Text));

	ftoa(temp, temp_Value, 1);

	lcdPrintStr(&lcd,(uint8_t*)temp_Value, sizeof(temp_Value));

	if(sensorTemp.temperatureUnit == C)
		lcdPrintStr(&lcd,CUnit_Text, sizeof(CUnit_Text));
	else
		lcdPrintStr(&lcd,FUnit_Text, sizeof(FUnit_Text));

	lcdSetCursorPosition(&lcd, 0, 2);
	lcdPrintStr(&lcd,hum_Text, sizeof(hum_Text));

	temp = sensorTemp.humidity;

	ftoa(temp, temp_Value2, 2);

	lcdPrintStr(&lcd,(uint8_t*)temp_Value2, sizeof(temp_Value2));

	lcdPrintStr(&lcd,PorUnit_Text, sizeof(PorUnit_Text));

	return;

}
/*Las siguientes 3 funciones fueron tomadas desde Internet para solucionar
 * la necesidad de convertir de FLOAT a STRING
 *
 */

// Reverses a string 'str' of length 'len'
static void reverse(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

// Converts a given integer x to string str[].
// d is the number of digits required in the output.
// If d is more than the number of digits in x,
// then 0s are added at the beginning.
static int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}
static void ftoa(float n, char* res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    float fpart = n - (float)ipart;

    // convert integer part to string
    int i = intToStr(ipart, res, 0);

    // check for display option after point
    if (afterpoint != 0) {
        res[i] = '.'; // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter
        // is needed to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);

        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}

/**
  * @brief  Funcion destinada a contenedor de manejo de errores
  * @param  None
  * @retval None
  */
static void _API_FSM_Error_Handler(void)
{
	/* Turn LED2 on */
	BSP_LED_On(LED2);
	while (1)
	{
	}
}

