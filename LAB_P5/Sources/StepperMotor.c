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
* It manages the sequence control and the motor direction. If the push button is pressed, the motor
* rotates in the opposite direction.
* This driver uses GPIO functions to set PTDB[0:3] up for motor signals, and PTAD1 for the button.

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
#define MOTOR_PINS_MASK 0xF0
#define FIRST_STEP 0
#define LAST_STEP 7
#define A_MOTOR_PIN 4
#define B_MOTOR_PIN 5
#define C_MOTOR_PIN 6
#define D_MOTOR_PIN 7
/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/
void StepperMotor_WriteCurrentStep(void);
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
//Motor on-off
static char sgu8isOn = 1;

/*************************************************************************************************/
/*********************				   Exported Functions					**********************/
/*************************************************************************************************/

void StepperMotor_Init(void)
{
	//Use PTBD 0 to 3 for motor terminals
	GPIO_ConfigPortOutputsByMask(PORTB,MOTOR_PINS_MASK);
	//Use PTBD 1 for direction button
	GPIO_ConfigPinDirection(PORTA,BTN_PIN,INPUT);
	StepperMotor_WriteCurrentStep();
}

void StepperMotor_NextStep(void)
{
	if(sgu8isOn)
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
		StepperMotor_WriteCurrentStep();
	}
}

void StepperMotor_SetDirection(char u8Dir)
{
	sgu8left = u8Dir&0x01;
}

void StepperMotor_TurnOnOff(char u8isOn)
{
	sgu8isOn = u8isOn&0x01;
}

/*************************************************************************************************/
/*********************				    Private Functions					**********************/
/*************************************************************************************************/

void StepperMotor_WriteCurrentStep(void)
{
	char u8currentStep = sgpu8stepValues[sgu8index];
	GPIO_WriteBitToPin(PORTB,A_MOTOR_PIN,(u8currentStep&0x01));
	GPIO_WriteBitToPin(PORTB,B_MOTOR_PIN,(u8currentStep>>1)&0x01);
	GPIO_WriteBitToPin(PORTB,C_MOTOR_PIN,(u8currentStep>>2)&0x01);
	GPIO_WriteBitToPin(PORTB,D_MOTOR_PIN,(u8currentStep>>3)&0x01);
}

