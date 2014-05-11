/*! **********************************************************************************************
 * 2014  ITESM Campus Guadalajara. Laboratorio de Microcontroladores 
 *  
 *
 * @file:      main.c
 * @author(s):  
 * Abner Perez Haro A01224776
 * Juan Carlos Perez Castellanos A01225629
 * 
 * @brief (Theory of Operation)
 * First, this module initialises Stepper Motor, Speed Manager and Delay Manager ECU.
 * Then it enters to an infinite loop where:
 * -It checks the motor speed with Speed Manager,
 * -executes a delay according to the selected speed with Delay Manager
 * -and updates the stepper sequence and the motor direction with Stepper Motor.
 **************************************************************************************************/

/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "LcdManager.h"
#include "GPIO.h"
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************                  Static Variables                    **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Variables					**********************/
/*************************************************************************************************/


//PTA1 es PIN_FREC
//PTA2-3 ENABLE-RS
//LCD PTB 7654 - D7654


#define PIN_FREC 1
#define PortA 0
/*************************************************************************************************/
/*********************                  Static Constants                    **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************                  Global Constants                    **********************/
/*************************************************************************************************/
static unsigned char sgu8AdcValue;
static unsigned char sgu8SpeedLevel;
static unsigned char sgu8OldSpeedLevel;
static unsigned char new_freq = 0;
static char sgu8Dir; 
static char sgpu8_3bitLcdMsg[8];
static unsigned int contador_8ms = 0;
char muestreo_nuevo = 0;
char muestreo_anterior = 0;
static unsigned int diametro = 8000; //En 8 cm. con los ms se compensa
unsigned int count_to_send = 0;
/*************************************************************************************************/
/*********************				   Exported Functions					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************				    Private Functions					**********************/
/*************************************************************************************************/
void main(void)
{
	unsigned int u16count_of_ms;
	char u8RpmSpeed;

	EnableInterrupts;
	LcdManager_Init();
	//Configure input frequency pin
	(void) GPIO_ConfigPinDirection(PortA, PIN_FREC, 0);
	sgu8OldSpeedLevel = 0;
	//Configured for 8 ms
	SRTISC = 0x11;

	for(;;){
			LcdManager_UpdateTask();
			if((LcdManager_IsBusy() == 0) && (new_freq == 1))
			{
				//RPM Speed
				u16count_of_ms = (diametro/((count_to_send)*8));
				u16count_of_ms *= 10;
				sgpu8_3bitLcdMsg[0] = (char)(u16count_of_ms/100) + 0x30;
				u8RpmSpeed = (u16count_of_ms%100);
				sgpu8_3bitLcdMsg[1] = u8RpmSpeed/10 + 0x30;
				sgpu8_3bitLcdMsg[2] = '.';
				sgpu8_3bitLcdMsg[3] = u8RpmSpeed%10 + 0x30;
				sgpu8_3bitLcdMsg[4] = 'c';
				sgpu8_3bitLcdMsg[5] = 'm';
				sgpu8_3bitLcdMsg[6] = '/';
				sgpu8_3bitLcdMsg[7] = 's';
				LcdManager_WriteString(1,0,sgpu8_3bitLcdMsg,8);
				new_freq = 0;
			}
		__RESET_WATCHDOG();
	}	
} 
void frecuencia_total()
{
	
}

interrupt 23 void RTI_interrupt(void)
{
	//Clear flag
	SRTISC_RTIACK = 1;
	muestreo_nuevo = GPIO_ReadPin(PortA, PIN_FREC);
	//Este caso se dara unicamente cuando detecte la linea
	if((muestreo_nuevo==1) && (muestreo_anterior==0))
	{
		count_to_send = contador_8ms;
		new_freq = 1;
		contador_8ms = 0;		
		//Comienza cuenta y actualiza frecuencia
	}
	//Caso en que estara contando hasta detectar la linea de nuevo
	else
	{
		muestreo_anterior = muestreo_nuevo;
		contador_8ms+=1;
	}
}

