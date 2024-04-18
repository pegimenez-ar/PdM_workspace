/**
 ********************************************************************************
 * @file    API_lcd.c
 * @author  Ing. Pablo E. Gimenez - pegimenez@fi.uba.ar
 * @date    Apr 14, 2024
 * @brief   La libreria API_lcd implementa un driver para display LCD HD44780
 * 			como trabajo final para las materias de Programacion de
 * 			Microcontroles y Protocolos de Comunicacion de Microcontroladores
 * 			del CESE_222024
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "API_lcd.h"
#include "API_lcd_port.h"

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
static void _API_LCD_Delay(uint32_t delay);
static bool _API_LCD_Send8bits(lcdType_t* lcd, uint8_t value, uint8_t type);
static bool _API_LCD_Send4bits(lcdType_t* lcd, uint8_t value, uint8_t type);
static void API_LCD_Error_Handler(void);

/************************************
 * STATIC FUNCTIONS
 ************************************/
/**
 * @brief  Esta función genera un delay bloqueante
 * @param  Tiempo en milisegundos
 * @retval None
 */
static void _API_LCD_Delay(uint32_t delay)
{
	HAL_Delay(delay);

	return;
}

/**
 * @brief  Funcion que envia un comando de 8 bits al middleware del puerto I2C
 * @param  Puntero al handler del LCD
 * @param  Valor del dato a enviar
 * @param  Tipo de dato: Comando (COMMAND) o Dato (DATA)
 * @retval None
 */
static bool _API_LCD_Send8bits(lcdType_t* lcd, uint8_t value, uint8_t type)
{
	if(lcd == NULL)
			API_LCD_Error_Handler();

	bool returnValue = false;

	//Se aplica mascara para Nibble superior e inferior
	uint8_t h_nibble = (value & HIGHER_NIBBLE);
	uint8_t l_nibble = ((value << LOWER_NIBBLE) & HIGHER_NIBBLE);

	//Tomo la respuesta de cada comando y comparo entre ellas.
	//En caso de error en una se pondrá el valor de retorno en true
	//indicando falla

	returnValue |= _API_LCD_Send4bits(lcd, h_nibble, type);
	returnValue |= _API_LCD_Send4bits(lcd, l_nibble, type);

	return returnValue;

}

/**
 * @brief  Funcion que envia un comando de 4 bits al middleware del puerto I2C
 * @param  Puntero al handler del LCD
 * @param  Valor del dato a enviar
 * @param  Tipo de dato: Comando (COMMAND) o Dato (DATA)
 * @retval None
 */
static bool _API_LCD_Send4bits(lcdType_t* lcd, uint8_t value, uint8_t type)
{
	if(lcd == NULL)
			API_LCD_Error_Handler();

	bool returnValue = false;

	if(API_LCD_PORT_Transmit(lcd , (value | EN | type | lcd->backlight) ))
		returnValue = true;

	_API_LCD_Delay(1);

	if(API_LCD_PORT_Transmit(lcd , (value | type | lcd->backlight) ))
			returnValue = true;

	return returnValue;

}

/**
 * @brief  Funcion diseña para container de error
 * @param  None
 * @retval None
 */
static void API_LCD_Error_Handler(void)
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
 * @brief	Función que genera la inicialización del LCD
 * @param	*lcd: Puntero a la variable de tipo lcd_t para manejo del dispositivo
 * @param  	*hi2c: Handler del puerto I2C
 * @param	address: Dirección del LCD en puerto I2C
 * @param	columns: Cantidad de colunmas que cuenta el display
 * @param	rows: Cantidad de filas del display
 * @param	character: Configura entre 5x8 dots o 5x10 dots
 * @param	bitmode: Configuracion del modo de trabajo 4bits u 8bits
 * @retval	un valor del tipo lcdStatusType_t indicando si fue Ok o el error
 */
lcdStatusType_t lcdInit(lcdType_t *lcd, I2C_HandleTypeDef *hi2c1, uint8_t address, uint8_t columns, uint8_t rows, uint8_t character, uint8_t bitmode)
{
	if(lcd == NULL || hi2c1 == NULL || address == 0x00)
		return LCD_ERROR;

	uint8_t _displayFunction = 0x00;

	//Se definen parametros por default
	const uint8_t _displayControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKCURSOROFF;		//Default: LCD On, Cursor Off, Blinking Off
	const uint8_t _displayMode = LCD_ENTRYLEFT;

	//Se inicializa los parametros del tipo puntero.
	lcd->hi2c1 = hi2c1;
	lcd->rows = rows;
    lcd->columns = columns;
    lcd->address = address;
    lcd->backlight = LCD_BACKLIGHT; 	//Default backlight On
    lcd->bitmode = bitmode;

    //Se determina parametros del display
    if(rows > 1)
    	_displayFunction |= LCD_2LINE;

    if(character != 0)
    	_displayFunction |= LCD_5x10DOTS;


    //Inicialiazamos el puerto I2C. Si devuelve error se cancela la secuencia y se retorna error
    if(API_LCD_PORT_Init(lcd))
    	return LCD_ERROR;

    /* A partir de aca se realiza la configuración que se indica en la pagina 45 & 46 del
     * datasheet del HD44780.
     */

    _API_LCD_Delay(DELAY_50_ms);
    _API_LCD_Send4bits(lcd , LCD_INIT1 , COMMAND);
    _API_LCD_Delay(DELAY_5_ms);
    _API_LCD_Send4bits(lcd , LCD_INIT1 , COMMAND);
    _API_LCD_Delay(DELAY_1_ms);
    _API_LCD_Send4bits(lcd , LCD_INIT1 , COMMAND);
    _API_LCD_Delay(DELAY_10_ms);
    _API_LCD_Send4bits(lcd , LCD_FUNCTIONSET , COMMAND);
    _API_LCD_Delay(DELAY_10_ms);

    _API_LCD_Send8bits(lcd , (LCD_FUNCTIONSET | _displayFunction) , COMMAND);
    _API_LCD_Delay(DELAY_1_ms);
    _API_LCD_Send8bits(lcd , (LCD_DISPLAYCONTROL | LCD_DISPLAYOFF) , COMMAND);
    _API_LCD_Delay(DELAY_1_ms);
    _API_LCD_Send8bits(lcd , LCD_CLEARDISPLAY , COMMAND);
    _API_LCD_Delay(DELAY_2_ms);
    _API_LCD_Send8bits(lcd , (LCD_ENTRYMODESET | _displayMode) , COMMAND);
    _API_LCD_Delay(DELAY_1_ms);

    //Se procede a encender el display
    _API_LCD_Send8bits(lcd , (LCD_DISPLAYCONTROL | _displayControl) , COMMAND);
    _API_LCD_Delay(DELAY_1_ms);

    return LCD_OK;
}

/**
 * @brief  Funcion que ejecuta funcion Clear del display
 * @param  Puntero al handler del LCD
 * @retval None
 */
void lcdClear(lcdType_t *lcd)
{
	if(lcd == NULL)
		API_LCD_Error_Handler();

	_API_LCD_Send8bits(lcd , LCD_CLEARDISPLAY , COMMAND);
	_API_LCD_Delay(DELAY_2_ms);

	return;
}

/**
 * @brief  Funcion que ejecuta funcion Retunr del display
 * @param  Puntero al handler del LCD
 * @retval None
 */
void lcdReturnHome(lcdType_t *lcd)
{
	if(lcd == NULL)
			API_LCD_Error_Handler();

	_API_LCD_Send8bits(lcd , LCD_RETURNHOME , COMMAND);
	_API_LCD_Delay(DELAY_2_ms);

	return;
}

/**
 * @brief  Funcion que setea el parametro backligth y enciende
 * @param  Puntero al handler del LCD
 * @retval None
 */
void lcdBacklight(lcdType_t *lcd)
{
	if(lcd == NULL)
		API_LCD_Error_Handler();

	lcd->backlight = LCD_BACKLIGHT;
	_API_LCD_Send4bits(lcd , LCD_BACKLIGHT , COMMAND);

	return;
}

/**
 * @brief  Funcion que desetea el parametro backligth y enciende
 * @param  Puntero al handler del LCD
 * @retval None
 */
void lcdNoBacklight(lcdType_t *lcd)
{
	if(lcd == NULL)
		API_LCD_Error_Handler();

	lcd->backlight = LCD_NOBACKLIGHT;
	_API_LCD_Send4bits(lcd , LCD_NOBACKLIGHT , COMMAND);

	return;
}

/**
 * @brief  Funcion que enciende pantalla
 * @param  Puntero al handler del LCD
 * @retval None
 */
void lcdDisplayOn(lcdType_t *lcd)
{
	if(lcd == NULL)
		API_LCD_Error_Handler();

	_API_LCD_Send8bits(lcd , (LCD_DISPLAYCONTROL | LCD_DISPLAYON) , COMMAND);

	return;
}

/**
 * @brief  Funcion que enciende pantalla
 * @param  Puntero al handler del LCD
 * @retval None
 */
void lcdDisplayOff(lcdType_t *lcd)
{
	if(lcd == NULL)
		API_LCD_Error_Handler();

	_API_LCD_Send8bits(lcd , (LCD_DISPLAYCONTROL | LCD_DISPLAYOFF) , COMMAND);

	return;
}

/**
 * @brief  Funcion que asigna la posicion del cursor o direccion de memoria
 * @param  Puntero al handler del LCD
 * @param  Valor de columna a posicionar. Valores de 0 a cantidad definida
 * @param  Valor de fila a posicionar. Valores de 0 a cantidad definida
 * @retval None
 */
void lcdSetCursorPosition(lcdType_t *lcd, uint8_t column, uint8_t row)
{
	if(lcd == NULL || row > lcd->rows || column > lcd->columns)
		API_LCD_Error_Handler();

	uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };

	uint8_t data = LCD_SETDDRAMADDRESS | (column + row_offsets[row]);

	_API_LCD_Send8bits(lcd, data, COMMAND);

}
/**
 * @brief  Funcion que imprime un string o mejor una cadena de caracters
 * @param  Puntero al handler del LCD
 * @param  Puntero a la cadena de caracteres
 * @param  Largo de la cadena
 * @retval None
 */
void lcdPrintStr(lcdType_t *lcd, const uint8_t * data, uint8_t length)
{
	if(lcd == NULL || data == NULL)
		API_LCD_Error_Handler();

    for (uint8_t i = 0; i < length-1; i++) {
        lcdPrintChar(lcd, data[i]);

        }

}

/**
 * @brief  Funcion que imprime un caracter en el display en la posicion actual
 * @param  Puntero al handler del LCD
 * @param  Caracter en codigo ascii a imprimir
 * @retval None
 */
void lcdPrintChar(lcdType_t *lcd, uint8_t data)
{
	if(lcd == NULL)
		API_LCD_Error_Handler();

	_API_LCD_Send8bits(lcd, data, DATA);

	return;

}


