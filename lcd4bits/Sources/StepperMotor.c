/*! **********************************************************************************************
* 2014  ITESM Campus Guadalajara. Laboratorio de Microcontroladores 
*  
*
* @file:      StepperMotor.c
* @author(s):  
* Abner Perez Haro A01224776
* Juan Carlos Perez Castellanos A01225629
* 
* @brief (Theory of Operation)
* It manages the sequence control and the motor direction. It checks whether the button pin is pressed,
* and when this happens, the driver changes the sequence direction. Moreover, it reports to Delay Manager
* to make a delay, to remove bouncing. 
* This divers uses GPIO functions to set up PTDB[0:3] for motor signals, and PTAD1 for the button.

**************************************************************************************************/

/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
#include "GPIO.h"
#include "DelayManager.h"

/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
#define BTN_PIN 1
#define MOTOR_PINS_MASK 0x0F
#define FIRST_STEP 0
#define LAST_STEP 7
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

/*************************************************************************************************/
/*********************                  Static Constants                    **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************                  Global Constants                    **********************/
/*************************************************************************************************/
//Values for half step sequence
static char sgpu8stepValues[8] = {0x01,0x03,0x02,0x06,0x04,0x0C,0x08,0x09}; 
//Current direction of the motor
static char sgu8left = 1;
//Current step
static char sgu8index = 0;

/*************************************************************************************************/
/*********************				   Exported Functions					**********************/
/*************************************************************************************************/

void StepperMotor_Init(void)
{
	//Use PTBD 0 to 3 for motor terminals
	GPIO_ConfigPortDirection_Mask(PORTB,MOTOR_PINS_MASK);

	//Use PTBD 1 for direction button
	GPIO_WritePort(PORTB,sgpu8stepValues[sgu8index]);
}

void StepperMotor_NextStep(void)
{
	//Check motor current direction
	if(sgu8left)
	{
		//Left
		if(sgu8index == LAST_STEP)
		{
			sgu8index = FIRST_STEP;
		}
		else
		{
			sgu8index++;
		}
	}
	else
	{
		//Right
		if(sgu8index == FIRST_STEP)
		{
			sgu8index = LAST_STEP;
		}
		else
		{
			sgu8index--;
		}
	}
	
	//Update sequence status in the motor port pins
	GPIO_WritePort(PORTB,sgpu8stepValues[sgu8index]);
}

char StepperMotor_RefreshDirection(void)
{
	//Read button pin status
	if(GPIO_ReadPin(PORTA,BTN_PIN))
	{
		sgu8left = (~sgu8left)&0x01;
		//Execute a de-bouncing delay 
		DelayManager_SetDebouncingDelay();
		DelayManager_Delay();
	}
	return sgu8left;
}



/*************************************************************************************************/
/*********************				    Private Functions					**********************/
/*************************************************************************************************/
