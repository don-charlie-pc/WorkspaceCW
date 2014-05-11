/*! **********************************************************************************************
* 2014  ITESM Campus Guadalajara. Laboratorio de Microcontroladores 
*  
*
* @file:      main.c
* @author(s):  
* 
* Juan Carlos Perez Castellanos A01225629
* 
* @brief (Theory of Operation)
* First, this module initialises Stepper Motor, Speed Manager, Encoder Manager and Delay Manager.
* 
* Then it enters to an infinite loop where:
* -It checks the motor current speed with Speed Manager.
* -Executes a delay according to the selected speed with Delay Manager. This delay runs parallel to the application,
*  and gives a chance to update the LCD, motor sequence and direction, without blocking the motor operation.
* -Then, it reads the current RPM speed, direction and number of travelled steps via Encoder Manager.
* -When the speed, direction or number of steps changes, and the LCD is not busy (writing a previous message), the LCD is updated.
* -Writing a message is a slow process, so each LcdManager_UpdateTask function call only perform one part of the total process.
*  We must call this function each iteration of the infinite loop, in order to guarantee the ending of LCD process.
*  
* The sent message format is: 
* 	FIRST LINE
* -The RPM motor speed. 
* -Travelled degrees. We perform the conversion from number of travelled steps to the corresponding degrees.
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
#include "LcdManager.h"
#include "EncoderManager.h"
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
#define THRESHOLD 10
#define MIN_SPEED 0
/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/
void Send_Msg(unsigned int u16SpeedRPM, char u8Dir, char u8Step);
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
char gpu8Line1[] = {"   RPM      °"};
char gpu8Line2[] = {"   "};
/*************************************************************************************************/
/*********************				   Exported Functions					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************				    Private Functions					**********************/
/*************************************************************************************************/
void main(void)
{
	//It was obtained from the ADC
	unsigned char u8SpeedLevel;
	//These were obtained from the Encoder 
	unsigned int u16SpeedRPM;
	unsigned int u16OldSpeedRMP;
	unsigned char u8Dir;
	unsigned char u8OldDir;
	unsigned char u8Step;
	unsigned char u8OldStep;
	
	//Initialise Modules
	SpeedManager_Init();
	DelayManager_Init();
	StepperMotor_Init();
	EncoderManager_Init();
	LcdManager_Init();
	
	EnableInterrupts;
	
	//Reset old values
	u16OldSpeedRMP = 0;
	u8OldDir = 0;
	u8OldStep = 0;
	
	//Initial delay for the lowest speed
	DelayManager_SetDelayBySpeed(MIN_SPEED);
	DelayManager_Delay();
	
	for(;;){
		
		u8SpeedLevel = SpeedManager_ReadSpeedLevel();
		if(!DelayManager_IsBusy())
		{
			//Update current step and motor direction as long as the motor delay is not busy
			StepperMotor_NextStep();
			DelayManager_SetDelayBySpeed(u8SpeedLevel);
			DelayManager_Delay();
			StepperMotor_RefreshDirection();
		}
		
		//Get encoder information
		EncoderManager_GetData(&u16SpeedRPM,&u8Dir,&u8Step);
		
		//Update LCD state machine
		LcdManager_UpdateTask();
		
		//Refresh message only if there is new information from the encoder
		if((u16SpeedRPM != u16OldSpeedRMP)||(u8Dir != u8OldDir)||(u8Step != u8OldStep))
		{
			if(!LcdManager_IsBusy()) //LCD must not be busy writing a previous message
			{
				Send_Msg(u16SpeedRPM,u8Dir,u8Step);
				
				u16OldSpeedRMP = u16SpeedRPM;
				u8OldDir = u8Dir;
				u8OldStep = u8Step;
			}			
		}
		__RESET_WATCHDOG();
	}	
} 

void Send_Msg(unsigned int u16SpeedRPM, char u8Dir, char u8Step)
{
	char u8SpeedRPM;
	unsigned int u16Degrees;
	char u8Degrees;
	
	//Write Line 1-------------------------------------------
	//RPM Speed
	gpu8Line1[0] = (char)(u16SpeedRPM/100) + 0x30; //Hundred
	u8SpeedRPM = (u16SpeedRPM%100);
	gpu8Line1[1] = u8SpeedRPM/10 + 0x30; //Tens
	gpu8Line1[2] = u8SpeedRPM%10 + 0x30; //Units
	
	//Degrees
	u16Degrees = u8Step * 75; //Convert number of steps to degrees.
	gpu8Line1[7] = (char)(u16Degrees/1000) + 0x30; //Hundred 
	u16Degrees = (u16Degrees%1000);
	gpu8Line1[8] = (char)(u16Degrees/100) + 0x30; //Tens
	u8Degrees = (u16Degrees%100);
	gpu8Line1[9] = (u8Degrees/10) + 0x30; //Units
	gpu8Line1[11] = (u8Degrees%10) + 0x30; //Decimal
	
	LcdManager_WriteString(1,0,gpu8Line1,13);

	//Write Line 2-------------------------------------------
	//Direction
	if(u8Dir)
	{
		gpu8Line2[0] = 'I';
		gpu8Line2[1] = 'Z';
		gpu8Line2[2] = 'Q';
	}
	else
	{
		gpu8Line2[0] = 'D';
		gpu8Line2[1] = 'E';
		gpu8Line2[2] = 'R';
	}
	LcdManager_WriteString(2,4,gpu8Line2,3);
}

