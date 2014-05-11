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
#include "MTIM.h"
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
#define NOT_ENOUGH_TIME -1
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

/*************************************************************************************************/
/*********************				   Exported Functions					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************				    Private Functions					**********************/
/*************************************************************************************************/

void main(void) {
	
	char u8speedCurrent = 0;
	char u8speedLast = 0;
	char u8dir = 0;
	char u8AdcValue = 0;
	unsigned int u16remainderDelay = 0; //Micro sec
	int u16motorDelay = 0; //Micro sec
	char u8notEnoughTimeState = 0;
	
	//Set up ECU Layer 
	EnableInterrupts;
	StepperMotor_Init();
	SpeedManager_Init();
	DelayManager_Init();
	LcdManager_Init();
	
	TPM_Init();
	TPM_SetMicroSecPFlag(10000);
	TPM_SetLimitFlags(3);
	TPM_Start();
	
	
  
	
	
	
	
	for(;;)
	{
		u8speedCurrent = SpeedManager_ReadSpeed(&u8AdcValue);
		if(TMP_isRunning()==0)
		{
			(void)LcdManager_Write(u8speedCurrent,u8AdcValue,1,0);
			TPM_Start();
		}
			
		__RESET_WATCHDOG();
	  
	  /*u8speedLast = u8speedCurrent;
	  u8speedCurrent = SpeedManager_ReadSpeed(&u8AdcValue);
	  u16motorDelay = DelayManager_SpeedtoMicroSeg(u8speedCurrent);
	  //Check input if it is necessary to change the motor direction
	  u8dir = StepperMotor_RefreshDirection();
	  
	  
	  
	  if(TMP_isRunning()==0 && (u8speedLast != u8speedCurrent))
	  {
		  //If the value of this function is minus 1, there is not enough time
		  u16remainderDelay = LcdManager_Write(u8speedCurrent,u8AdcValue,u8dir,u16motorDelay);
		  if(NOT_ENOUGH_TIME == u16remainderDelay)
		  {
			  u8notEnoughTimeState = 1;
		  }
		  else if (u16motorDelay < u16remainderDelay)
		  {
			  u16motorDelay -= u16remainderDelay;
			  //Wait before change to the next step
			  
			  (void)DelayManager_SetDelayByMicroSeg(u16motorDelay);
			  
			  
			  DelayManager_Delay();
		  } 
	  }
	  
	  else
	  {
		  DelayManager_SetDelayByMicroSeg(u16motorDelay);
		  DelayManager_Delay();
	  }
	  
	  if(u8notEnoughTimeState != 0)
	  {
		  u8notEnoughTimeState = 0;
		  u16remainderDelay=LcdManager_RemainderTask();
		  u16motorDelay -= u16remainderDelay;
		  (void)DelayManager_SetDelayByMicroSeg(u16motorDelay);
		  DelayManager_Delay();
	  }*/
	  
	  //StepperMotor_NextStep();
	   
	}
}
