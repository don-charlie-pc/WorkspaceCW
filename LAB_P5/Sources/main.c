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
* First, this module initialises Stepper Motor, Speed Manager and Delay Manager.
* 
* Then it enters to an infinite loop where:
* -It checks the motor current speed with Speed Manager.
* -Executes a delay according to the selected speed with Delay Manager. This delay runs parallel to the application,
*  and gives a chance to update the LCD, motor sequence and direction, without blocking the motor operation.
* -When the speed or direction changes, and the LCD is not busy (writing a previous message), the LCD is updated.
* -Writing a message is a slow process, so each LcdManager_UpdateTask function call only perform one part of the total process.
*  We must call this function each iteration of the infinite loop, in order to guarantee the ending of LCD process.
*  
* The sent message format is: 
* 	FIRST LINE
* -The RPM motor speed. We perform the conversion from speed level to RPM.
* -The ADC voltage. We perform the conversion from ADC value to the corresponding voltage.
* 	SECOND LINE
* -Motor direction: Right or left.
**************************************************************************************************/

/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "StepperMotor.h"
#include "SpeedManager.h"
#include "DelayManager.h"
#include "MTIM.h"
#include "SerialManager.h"
#include "RTI.h"

/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
#define THRESHOLD 10
/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/
void Send_Msg(char u8AdcValue);
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
//Messages for each line  
char gpu8Msg[] = {" .  V    us\r\n"}; 
unsigned char gu8adcVoltage=0;
unsigned int gu16adcVoltage=0;
char sgu8count = 0;
/*************************************************************************************************/
/*********************				   Exported Functions					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************				    Private Functions					**********************/
/*************************************************************************************************/
void main(void)
{
	unsigned char u8AdcValue;
	unsigned char u8Dir;
	unsigned char u8isOn;
	unsigned int u16RpmSpeed;
	
	EnableInterrupts;
	//Initialise Modules
	SpeedManager_Init();
	DelayManager_Init();
	StepperMotor_Init();
	SerialManager_Init();

	//Initial delay for lower speed
	(void)DelayManager_SetDelayBy10Rpm(2000);
	DelayManager_Delay();
	
	//Initialise ten seconds timer
	RTI_Init();
	RTI_Start();
	MTIM_Init();
	(void)MTIM_SelectPrescaler(2);
	
	for(;;){
		
		MTIM_Start();
		SerialManager_GetValues(&u8Dir,&u8isOn,&u16RpmSpeed);
		
		if(!DelayManager_IsBusy())
		{
			//Update current step and motor direction as long as the motor delay is not busy
			StepperMotor_TurnOnOff(u8isOn);
			StepperMotor_SetDirection(u8Dir);
			StepperMotor_NextStep();
			if(u8isOn)
			{
				(void)DelayManager_SetDelayBy10Rpm(u16RpmSpeed);
				DelayManager_Delay();
			}
		}
		
		SerialManager_UpdateTask();
		if(!RTI_isRunning())
		{
			RTI_Start();
			if(!SerialManager_IsBusy())
			{
				(void)SpeedManager_ReadSpeedLevel(&u8AdcValue);
				Send_Msg(u8AdcValue);
			}
		}
		__RESET_WATCHDOG();
		MTIM_Stop();
		sgu8count = MTIM_GetCount();
		MTIM_Reset();
	}	
} 

void Send_Msg(char u8AdcValue)
{
	
	//ADC value
	__RESET_WATCHDOG();
	gu16adcVoltage = (unsigned int)(129*u8AdcValue);
	gu16adcVoltage /= 100;
	gpu8Msg[0] = (gu16adcVoltage/100) + 0x30; //Units
	gu8adcVoltage = (gu16adcVoltage%100);
	gpu8Msg[2] = gu8adcVoltage/10 + 0x30; //Tenth
	gpu8Msg[3] = gu8adcVoltage%10 + 0x30; //Hundredth
	__RESET_WATCHDOG();
	//SerialManager_WriteString(&gpu8Msg,7);
	
	gpu8Msg[6] = sgu8count/100+0x30;
	sgu8count = sgu8count%100;
	gpu8Msg[7] = sgu8count/10+0x30;
	gpu8Msg[8] = sgu8count%10+0x30;
	SerialManager_WriteString(&gpu8Msg,13);
	__RESET_WATCHDOG();
}

