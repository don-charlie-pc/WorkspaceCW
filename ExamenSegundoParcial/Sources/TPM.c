/*! **********************************************************************************************
* 2014  ITESM Campus Guadalajara. Laboratorio de Microcontroladores 
*  
*
* @file:      TPM.c
* @author(s):  
* Abner Pérez Haro A01224776
* Juan Carlos Pérez Castellanos A01225629
* 
* @brief (Theory of Operation)
* This driver set this initial configuration for TPM:
*	-> Enable flag interrupt
*	-> No clock source selected
*	-> Prescale divisor 1/4
* It allows to configure the interrupt flag time in us, and set extra interrupt flags to obtain longer
* delays. It is possible to start, stop and know whether the timer is running or not. 
**************************************************************************************************/


/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
#include "derivative.h"
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
#define INITIAL_CONFIG 0b01000010
#define START_MASK 0x08
#define STOP_MASK 0x0F7
#define MAX_NUM_OF_FLAGS 200
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
static char sgu8flagCounter = 0;
static char sgu8extraFlagsPDelay = 0;
static char sgu8running = 0;
static char sgu8stopOnOverflow = 0;
static unsigned int sgu16channel0Val = 0;
static unsigned int sgu16channel1Val = 0;

//For step counter
static signed char sgs8stepCount = 0;
static char sgu8dir = 0;

/*************************************************************************************************/
/*********************				   Exported Functions					**********************/
/*************************************************************************************************/

void TPM_Init(void)
{
	//Set initial configuration:
	//*Enable flag interrupt
	//*No clock source selected
	//*Prescale divisor 1/4
	TPMSC = INITIAL_CONFIG;
	
	//Reset Timer Module
	TPMMODH = 0;
	TPMMODL = 0;
	
	//Reset flag counters
	sgu8flagCounter = 0;
	sgu8extraFlagsPDelay = 0;
	
	//Timer is stopped
	sgu8running = 0;
}

void TPM_SetExtraFlags(char u8numFlags)
{
	if(!sgu8running)
	{
		sgu8extraFlagsPDelay = u8numFlags;
	}
}

void TPM_SetModuloVal(unsigned int u16value)
{
	//Store u16microSec in timer module
	TPMMODL = (char)u16value;
	TPMMODH = (char)(u16value >> 8);
}

void TPM_Start(void)
{
	if(!sgu8running)
	{
		//Bus clock as source selected
		TPMSC |= START_MASK;
		//Toggle running flag
		sgu8running = (~sgu8running)&0x01;
	}	
}

void TPM_Stop(void)
{
	if(sgu8running)
	{
		//No clock source selected
		TPMSC &= STOP_MASK;
		//Toggle running flag
		sgu8running = (~sgu8running)&0x01;
	}
}

void TPM_Reset(void)
{
	TPMCNT = 0;
}

char TPM_IsRunning(void)
{
	return sgu8running;
}

void TPM_StopOnOverflow(char u8stop){
	sgu8stopOnOverflow = u8stop;
}

void TPM_SetPrescaler(char u8prescaler)
{
	TPMSC |= (0x07 & u8prescaler); 
}

void TPM_EnableInputCaptureCH0(void)
{
	TPMC0SC = 0b01000100;
}

void TPM_EnableInputCaptureCH1(void)
{
	TPMC1SC = 0b01000100;
}

unsigned int TPM_getCH0Val(void)
{
	return sgu16channel0Val;
}

unsigned int TPM_getCH1Val(void)
{
	return sgu16channel1Val;
}

void TPM_ResetStepCounter(void)
{
	sgs8stepCount = 0;
	sgu8dir = 0;
}

char TPM_GetStep(void)
{
	return sgs8stepCount;
}

void TPM_SetDir(char u8dir)
{
	sgu8dir = u8dir;
}

/*************************************************************************************************/
/*********************				    Private Functions					**********************/
/*************************************************************************************************/

interrupt 7 void TPM_Overflow_ISR(void)
{
	TPMSC_TOF = 0;
	if(sgu8flagCounter < sgu8extraFlagsPDelay)
	{
		sgu8flagCounter++;
	}
	sgu8flagCounter = 0;
	if (sgu8stopOnOverflow) {
		TPM_Stop();
	}
}

interrupt 5 void TPMCH0_ISR(void)
{
	//Clear channel flag
	(void)TPMC0SC;
	TPMC0SC_CH0F = 0;
	sgu16channel0Val = TPMC0V;
	
	if(sgu8dir)
	{
		if(sgs8stepCount < 48)
		{
			sgs8stepCount++;
		}
		else
		{
			sgs8stepCount = 0;
		}
	}
	else
	{
		if(sgs8stepCount >= 0)
		{
			sgs8stepCount--;
		}
		else
		{
			sgs8stepCount = 47;
		}
	}
	
	
}

interrupt 6 void TPMCH1_ISR(void)
{
	//Clear channel flag
	(void)TPMC1SC;
	TPMC1SC_CH1F = 0;
	sgu16channel1Val = TPMC1V;
}
