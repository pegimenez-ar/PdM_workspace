 /**
  ******************************************************************************
  * @file    API_debounce.c
  * @author  Pablo Gimenez
  * @brief   Archivo de la implementacion de las funciones de comunicacion UART
  ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "API_uart.h"

/* Definess ------------------------------------------------------------------*/
#define UART_TIMEOUT 250

#define CONFIGURATION_PARAMETERS_TEXT "CONFIGURATION PARAMETERS\n\
BAUD RATE: 19200\nDATA BITS: 8\nSTOP BITS: 1\nPARITY: NONE\n\
HW CONTROl: NONE\n"

/* Private typedef -----------------------------------------------------------*/
/* Private Variables Declaration ----------------------------------------------*/

/* UART handler declaration */
static UART_HandleTypeDef UartHandle;


/* Private Functions Declaration ---------------------------------------------*/
static void API_UART_Error_Handler(void);

/* Desarrollo de las funciones solicitadas */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Funcion que inicializa la Maquina de Estados Finitos con un estado definido
  * 		por default
  * @param  None
  * @retval None
  */
bool_t uartInit(){

	/*##-1- Configure the UART peripheral ######################################*/
	/* Put the USART peripheral in the Asynchronous mode (UART Mode) */
	/* UART configured as follows:
	      - Word Length = 8 Bits (7 data bit + 1 parity bit) :
		                  BE CAREFUL : Program 7 data bits + 1 parity bit in PC HyperTerminal
	      - Stop Bit    = One Stop bit
	      - Parity      = ODD parity
	      - BaudRate    = 9600 baud
	      - Hardware flow control disabled (RTS and CTS signals) */

	UartHandle.Instance        = USARTx;

	UartHandle.Init.BaudRate   = 19200;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits   = UART_STOPBITS_1;
	UartHandle.Init.Parity     = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode       = UART_MODE_TX_RX;
	UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

	if (HAL_UART_Init(&UartHandle) != HAL_OK)
	{
		/* Initialization Error */
		return UART_INIT_NOK;
	}

	uint8_t text[] = CONFIGURATION_PARAMETERS_TEXT;

	uartSendString(text);

	return UART_INIT_OK;

}

/**
  * @brief  Funcion que envia un texto por el puerto UART definido. No se indica el largo porqeu se procesa
  * 		hasta encontrar el \0 en el texto provisto
  * @param  Puntero al texto a enviar
  * @retval None
  */
void uartSendString(uint8_t * pstring){

	uint16_t size = 0;

	/* Se obtiene el largo del texto a enviar buscando \0 */
	while(*(pstring+size)){
		size++;
	}

	/* Se envia el texto utilizando la funcion definida */
	uartSendStringSize(pstring, size);

}

/**
  * @brief  Funcion que envia un texto por el puerto UART definido. Se indica el largo del texto (en caracteres)
  * 		que se desea enviar. Si no se trabaja con cuidado el largo del texto puede enviar parametros no deseados
  * @param  Puntero al texto a enviar y el largo del texto en caracteres a enviar
  * @retval None
  */
void uartSendStringSize(uint8_t * pstring, uint16_t size){

	if(HAL_UART_Transmit(&UartHandle, pstring, size, UART_TIMEOUT) != HAL_OK){

		/* Si la transmision genero error se llama a Error_Handler() */
		API_UART_Error_Handler();
	}

}

/**
  * @brief  Funcion que receibe un texto por el puerto UART definido. Se indica el puntero al buffer donde almacenar el
  * 		texto recibido y la cantidad de caracteres a leer. Si llega el TIMEOUT se va a error pero como no esta definido
  * 		el uso en el ejercicio se lo plantea asi.
  * @param  Puntero al texto a recibir y el largo del texto en caracteres a recibir
  * @retval None
  */
void uartReceiveStringSize(uint8_t * pstring, uint16_t size){

	if(HAL_UART_Receive(&UartHandle, pstring, size, UART_TIMEOUT) != HAL_OK){

		/* Si la recepcion genero error se llama a Error_Handler() */
		API_UART_Error_Handler();
	}
}

/**
  * @brief  Funcion que maneja los errores del API. Simplemente enciende el LED2 de la board y va a loop infinito
  * @param  None
  * @retval None
  */
static void API_UART_Error_Handler(void)
{
	/* Turn LED2 on */
	BSP_LED_On(LED1);
	while (1)
	{
	}
}


