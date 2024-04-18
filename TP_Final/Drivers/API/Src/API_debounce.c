/**
 ********************************************************************************
 * @file    API_debounce.c
 * @author  Ing. Pablo E. Gimenez - pegimenez@fi.uba.ar
 * @date    Apr 14, 2024
 * @brief   La libreria API_debounce implementa un antirebote para el boton de
 * 			usuario de la placa nucleo-144. Se desarrollo como trabajo final
 * 			para las materias de Programacion de Microcontroles y Protocolos
 * 			de Comunicacion de Microcontroladores
 * 			del CESE_222024
 ********************************************************************************
 */
/************************************
 * INCLUDES
 ************************************/
#include "API_debounce.h"

/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * PRIVATE TYPEDEFS
 ************************************/
typedef enum{
	BUTTON_UP,
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RAISING,
} debounceState_t;

/************************************
 * STATIC VARIABLES
 ************************************/
static debounceState_t estadoMEF;
static bool_t buttonPress = false;
static delay_t delayANTIBOUNCE, *p_delayANTIBOUNCE;

/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/
static void buttonPressed(void);
static void buttonReleased(void);
static void API_DEBOUNCE_Error_Handler(void);

/************************************
 * STATIC FUNCTIONS
 ************************************/
/**
  * @brief  Funcion que inicializa la Maquina de Estados Finitos con un estado definido
  * 		por default
  * @param  None
  * @retval None
  */
void debounceFSM_init(void){

	estadoMEF = BUTTON_UP;

	p_delayANTIBOUNCE = &delayANTIBOUNCE;

	delayInit( p_delayANTIBOUNCE , DELAY_BOUNCE);

}

/**
  * @brief  Funcion que actualiza el estado de la Maquina de Estados Finitos según haya cambios en la
  * 		entrada (pulsador presionado) o en base al tiempo transcurrido desde el cambio de estado
  * 		de la entrada en las anteriores ejecuciones
  * @param  None
  * @retval None
  */
void debounceFSM_update(void){

	switch(estadoMEF){

	case BUTTON_UP:
		if(BSP_PB_GetState(BUTTON_USER)){
			delayRead (p_delayANTIBOUNCE);
			estadoMEF = BUTTON_FALLING;
		}
		break;

	case BUTTON_FALLING:
		if(delayRead(p_delayANTIBOUNCE)){
			if(BSP_PB_GetState(BUTTON_USER)){
				estadoMEF = BUTTON_DOWN;
				buttonPressed();
			}else
				estadoMEF = BUTTON_UP;
		}
		break;

	case BUTTON_DOWN:
		if(!BSP_PB_GetState(BUTTON_USER)){
			delayRead (p_delayANTIBOUNCE);
			estadoMEF = BUTTON_RAISING;
		}
		break;

	case BUTTON_RAISING:
		if(delayRead (p_delayANTIBOUNCE)){
			if(!BSP_PB_GetState(BUTTON_USER)){
				estadoMEF = BUTTON_UP;
				buttonReleased();
			}else
				estadoMEF = BUTTON_DOWN;
		}
		break;

	/*CASO DEFAULT: En caso de no haber ejecutado una accion por los posibles
	 * estados anteriornes indica que la maquina de estado no se encuentra en
	 * un estado posible o válido por algun motivo por lo cual se llama a la
	 * funcion error
	 */
	default:
		API_DEBOUNCE_Error_Handler();
		break;
	}

}
/**
  * @brief	Funcion que indica si el pulsador fue presionado. Se coloca en TRUE (presionado) solamente cuando se solto el pulsador
  * 		es decir con un flanco descendente. Una vez que entrega TRUE por haber presionado limpia la variable privada de estado
  * @param  None
  * @retval	TRUE en caso que ha existido un pulso descendente del pulsador - FALSE cuando no se ha presionado o el pulsador sigue en estado presionado
  */
bool_t readKey(void){

	bool_t value = false;

	if(buttonPress){
		value = true;
		buttonPress = false;
	}

	return value;
}

/**
  * @brief  Funcion que ejecuta accion si se presiona LED (no implementado - utilizado)
  * @param  None
  * @retval None
  */
void buttonPressed(void){

	//BSP_LED_On(LED2);

}

/**
  * @brief  Funcion que realiza accion de cambiar variable global privada de boton presionado
  * @param  None
  * @retval None
  */

void buttonReleased(void){

	buttonPress = true;

}

static void API_DEBOUNCE_Error_Handler(void)
{
	/* Turn LED2 on */
	BSP_LED_On(LED1);
	while (1)
	{
	}
}


/************************************
 * GLOBAL FUNCTIONS
 ************************************/
