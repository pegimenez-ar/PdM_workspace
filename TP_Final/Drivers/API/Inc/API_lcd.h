/**
 ********************************************************************************
 * @file    API_lcd.h
 * @author  Ing. Pablo E. Gimenez - pegimenez@fi.uba.ar
 * @date    Apr 14, 2024
 * @brief   La libreria API_lcd implementa un driver para display LCD HD44780
 * 			como trabajo final para las materias de Programacion de
 * 			Microcontroles y Protocolos de Comunicacion de Microcontroladores
 * 			del CESE_222024
 ********************************************************************************
 */

#ifndef API_INC_API_LCD_H_
#define API_INC_API_LCD_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */
#include <stdbool.h>

/************************************
 * MACROS AND DEFINES
 ************************************/

/*Se definen la combinacion de comandos en base a la Tabla 6
 * de Instrucciones de la pagina 24 del datasheet del LCD
 * Hitachi HD44780U
 */

//Commandos
#define LCD_CLEARDISPLAY			0x01
#define LCD_RETURNHOME 				0x02
#define LCD_ENTRYMODESET 			0x04
#define LCD_DISPLAYCONTROL			0x08
#define LCD_CURSORDISPLAYSHIFT 		0x10
#define LCD_FUNCTIONSET				0x20
#define LCD_SETCGRAMADDRESS			0x40
#define LCD_SETDDRAMADDRESS			0x80
#define LCD_REDBUSSYFLAGADDRESS 	0x100
#define LCD_WRITEDATACGORDDRAM		0x200
#define LCD_READDATACGORDDRAM		0x300

//Opciones para Comando Display Entry Mode
#define LCD_ENTRYRIGHT 				0x00
#define LCD_ENTRYLEFT 				0x02
#define LCD_ENTRYSHIFTINCREMENT 	0x01
#define LCD_ENTRYSHIFTDECREMENT 	0x00

//Opciones para Comando Display On/Off Control
#define LCD_DISPLAYON 				0x04
#define LCD_DISPLAYOFF 				0x00
#define LCD_CURSORON 				0x02
#define LCD_CURSOROFF 				0x00
#define LCD_BLINKCURSORON			0x01
#define LCD_BLINKCURSOROFF			0x00

//Opciones para Comando Display/Cursor Shift
#define LCD_DISPLAYMOVE 			0x08
#define LCD_CURSORMOVE 				0x00
#define LCD_MOVERIGHT 				0x04
#define LCD_MOVELEFT 				0x00

//Opciones para Comando Function Set
#define LCD_8BITMODE 				0x10
#define LCD_4BITMODE 				0x00
#define LCD_2LINE 					0x08
#define LCD_1LINE 					0x00
#define LCD_5x10DOTS 				0x04
#define LCD_5x8DOTS 				0x00

//Opciones para Control Backlight
#define LCD_BACKLIGHT 				0x08
#define LCD_NOBACKLIGHT 			0x00

//Control Pines Funciones
#define EN 							0x04	// Enable bit
#define RW 							0x02	// Read = 0 / Write = 1 bit
#define RS 							0x01	// Registro Dato = 1 o Comando = 0

#define COMMAND						0x00
#define DATA						0x01

//Mascaras
#define HIGHER_NIBBLE 				0xF0
#define LOWER_NIBBLE 				4

#define LCD_INIT1					0x30
#define LCD_INIT2					0x20

//Definicion de los Delays
#define DELAY_50_ms		50
#define DELAY_10_ms		10
#define DELAY_5_ms		5
#define DELAY_2_ms		2
#define DELAY_1_ms		1


/************************************
 * TYPEDEFS
 ************************************/
typedef struct {
    I2C_HandleTypeDef* hi2c1;  		// Estructura I2C
    uint8_t rows;             		// Nro lineas del display
    uint8_t columns;           		// Nro columnas del display
    uint8_t address;           		// Direccion I2C
    uint8_t backlight;         		// Estado Backlight
    uint8_t bitmode;				// Interface datalength de 4bits o 8bits
} lcdType_t;

typedef enum
{
	LCD_OK,
	LCD_ERROR,
} lcdStatusType_t;

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
lcdStatusType_t lcdInit(lcdType_t *lcd, I2C_HandleTypeDef *hi2c1, uint8_t address, uint8_t columns, uint8_t rows, uint8_t character, uint8_t bitmode);
void lcdClear(lcdType_t *lcd);
void lcdReturnHome(lcdType_t *lcd);
void lcdBacklight(lcdType_t *lcd);
void lcdNoBacklight(lcdType_t *lcd);
void lcdDisplayOn(lcdType_t *lcd);
void lcdDisplayOff(lcdType_t *lcd);
void lcdSetCursorPosition(lcdType_t *lcd, uint8_t column, uint8_t row);
void lcdPrintStr(lcdType_t *lcd, const uint8_t * data, uint8_t length);
void lcdPrintChar(lcdType_t *lcd, uint8_t data);


#ifdef __cplusplus
}
#endif

#endif 
