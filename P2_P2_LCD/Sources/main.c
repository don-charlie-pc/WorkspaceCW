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
#include "StepperMotor.h"
#include "SpeedManager.h"
#include "DelayManager.h"
#include "LcdManager.h"
#include "TPM.h"
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

/*************************************************************************************************/
/*********************                  Static Constants                    **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************                  Global Constants                    **********************/
/*************************************************************************************************/
static unsigned char sgu8AdcValue;
static unsigned char sgu8SpeedLevel;
static unsigned char sgu8OldSpeedLevel;
static char sgu8Dir; 
static char sgpu8_3bitLcdMsg[3];
/*************************************************************************************************/
/*********************				   Exported Functions					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************				    Private Functions					**********************/
/*************************************************************************************************/
void main(void)
{
	unsigned int u16RpmSpeed;
	char u8RpmSpeed;
	unsigned int u16adcVoltage;
	char u8adcVoltage;
	
	EnableInterrupts;
	SpeedManager_Init();
	DelayManager_Init();
	StepperMotor_Init();
	LcdManager_Init();
	
	sgu8OldSpeedLevel = 0;
	(void)DelayManager_SetDelayBySpeed(0);
	DelayManager_Delay();
	
	for(;;){
		sgu8SpeedLevel = SpeedManager_ReadSpeedLevel(&sgu8AdcValue);
		if(DelayManager_IsBusy() == 0)
		{
			StepperMotor_NextStep();
			sgu8Dir = StepperMotor_RefreshDirection();
			(void)DelayManager_SetDelayBySpeed(sgu8SpeedLevel);
			DelayManager_Delay();
		}
		
		LcdManager_UpdateTask();
		if((sgu8SpeedLevel != sgu8OldSpeedLevel) && (LcdManager_IsBusy() == 0))
		{
			//RPM Speed
			u16RpmSpeed = ((sgu8SpeedLevel+1)*11)+1;
			sgpu8_3bitLcdMsg[0] = (char)(u16RpmSpeed/100) + 0x30;
			u8RpmSpeed = (u16RpmSpeed%100);
			sgpu8_3bitLcdMsg[1] = u8RpmSpeed/10 + 0x30;
			sgpu8_3bitLcdMsg[2] = u8RpmSpeed%10 + 0x30;
			LcdManager_WriteString(1,0,sgpu8_3bitLcdMsg,3);
			
			//ADC value
			/*u16adcVoltage = ((129*sgu8AdcValue)/100);
			sgpu8_3bitLcdMsg[0] = (u16adcVoltage/100) + 0x30;
			u8adcVoltage = (u16adcVoltage%100);
			sgpu8_3bitLcdMsg[1] = u8adcVoltage/10 + 0x30;
			sgpu8_3bitLcdMsg[2] = u8adcVoltage%10 + 0x30;
			LcdManager_WriteString(1,4,sgpu8_3bitLcdMsg,3);*/
			
			sgu8OldSpeedLevel = sgu8SpeedLevel;
		}
		__RESET_WATCHDOG();
	}	
} 

