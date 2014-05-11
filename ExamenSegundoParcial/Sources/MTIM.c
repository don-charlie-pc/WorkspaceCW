/*! **********************************************************************************************
* 2014  ITESM Campus Guadalajara. Laboratorio de Microcontroladores 
*  
*
* @file:      MTIM.c
* @author(s):
* Abner Perez Haro A01224776  
* Juan Carlos Perez Castellanos A01225629
* 
* @brief (Theory of Operation)
* It allows to configure the clock source, the prescaler, the interrupt, and write the comparison
* module of MTIM timer. It manages the MTIM interrupts. It is possible to start and stop this timer.
* When an interrupt flag is launched, MTIM will be automatically stopped and reseted. For getting 
* longer delays we can configure additional flags until this timer stops.  

**************************************************************************************************/

/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
#include "derivative.h"
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
#define INITIAL_STATUS_CONFIG 0b01110000
#define INITIAL_CLK_CONFIG 0
#define START_MASK 0xEF
#define STOP_MASK 0x10
#define NOT_RUNNING_MASK 0x10
#define RESET_MASK 0x20
#define MAX_PRESCALER_OPTION 8
#define CLEAR_PRESCALER_MASK 0xF0
#define MAX_CLK_OPTION 3
#define CLEAR_CLK_MASK 0xCF
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
static char sgu8flagCounter = 0;
static char sgu8extraFlagsPDelay = 0;
static char sgu8currentModulo = 0;
/*************************************************************************************************/
/*********************                  Static Constants                    **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************                  Global Constants                    **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************				   Exported Functions					**********************/
/*************************************************************************************************/

void MTIM_Init(void)
{
	/*Set initial status configuration:
		*Clear overflow flag 	bit7 = 0
		*Enable interrupt		bit6 = 1
		*Reset timer			bit5 = 1
		*Timer is stopped 		bit4 = 1
	*/
	MTIMSC = INITIAL_STATUS_CONFIG;
	
	/*Set initial status configuration:
		*Bus clock selected 	bit5:4 = 0b00
		*PRESCALER /1			bit3:0 = 0b0000
	*/
	
	MTIMCLK = INITIAL_CLK_CONFIG;
	
	//Reset Timer Module
	MTIMMOD = 0;
	
	//Reset flag counters
	sgu8flagCounter = 0;
	sgu8extraFlagsPDelay = 0;
}

void MTIM_Start(void)
{
	//Starts the Timer with the previously configured mode operation 
	MTIMMOD = sgu8currentModulo;
	MTIMSC &= START_MASK;
}

void MTIM_Stop(void)
{
	//Stops the Timer
	MTIMSC |= STOP_MASK;
}

char MTIM_IsRunning(void)
{
	//Returns 1 if MTIM is running and 0 if is not.
	if(MTIMSC & STOP_MASK)
	{
		return 0;
	}
	return 1;
}

void MTIM_Reset(void)
{
	//Resets the timer and set the default configuration of MTIM
	MTIMSC |= RESET_MASK;
}

char MTIM_SelectClkSource(char u8source)
{
	if(u8source <= MAX_CLK_OPTION)
	{
		//Change clock source
		MTIMCLK &= CLEAR_CLK_MASK;
		MTIMCLK |= (u8source << 4) & 0x30;
		return 1; 
	}
	return 0;
}

char MTIM_SelectPrescaler(char u8prescaler)
{
	if(u8prescaler <= MAX_PRESCALER_OPTION)
	{
		MTIMCLK &= CLEAR_PRESCALER_MASK;
		MTIMCLK |= (u8prescaler & 0x0F);
		return 1; //Return 1 if this PRESCALER is allowed
	}
	return 0;
}

void MTIM_SetTimerModulo(char u8value)
{	
	sgu8currentModulo = u8value;
	MTIMMOD = u8value;
}

void MTIM_SetExtraFlags(char u8numFlags)
{
	if(MTIMSC & NOT_RUNNING_MASK)
	{
		sgu8extraFlagsPDelay = u8numFlags;
	}
}

/*************************************************************************************************/
/*********************				    Private Functions					**********************/
/*************************************************************************************************/

interrupt 12 void MTIM_ISR(void)
{
	MTIMSC |= RESET_MASK;
	
	if(sgu8flagCounter < sgu8extraFlagsPDelay)
	{
		sgu8flagCounter++;
	}
	else
	{
		//Stop and reset when max number of flags per delay is reached
		sgu8flagCounter = 0;
		MTIMSC |= RESET_MASK;
		MTIMSC |= STOP_MASK;
	}
}
