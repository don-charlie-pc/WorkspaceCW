/*! **********************************************************************************************
* 2014  ITESM Campus Guadalajara. Laboratorio de Microcontroladores 
*  
*
* @file:      LcdManager.c
* @author(s):  
* Abner Perez Haro A01224776
* Juan Carlos Perez Castellanos A01225629
*
* @brief (Theory of Operation)
* 
* ---INIT process---
* First, INIT function sets MTIM up, and waits 20 milliseconds for LCD booting. After that, it changes to 4 bit mode.
* Then, it writes the required commands for LCD configuration, and finally changes to LCD IDLE state.
* 
* ---DELAYS---
* This configuration results in 100 us for interrupt time, that will be used as base time. Manipulating extra
* interrupt flags via MTIM_SetExtraFlags function, it is possible to obtain longer delays.
* This module uses three types of delays:
* -> 20 ms: LCD booting.
* -> 5 ms: For low pulse time to clear display.
* -> 100 us: For pulses of configuration commands and data sending.

* ---STATE MACHINE---
* LcdManager has a state machine which controls the writing process for non blocking delays, in order to use
* more peripherals simultaneously. The states are the following:
* 	INIT: When LCD is initialising.
* 	IDLE: State when LCD has been initialised, but there are no updated values to write. 
*		  LCD is not printing (Busy flag is clear). Until the BusyFlag is set, writing via LcdManager_WriteString function, 
*		  the state machine changes to SET_POSITION1. 
*	SET_POSITION1: Set the position on which the new value has to be displayed for line 1. Then, it changes to WRITE_CHAR1.
*	WRITE_CHAR1: Writes the new values to display (one value per LcdManager_UpdateTask call). When it finishes to write
*	all the characters, the state machine proceeds.
*	SET_POSITION2: Set the position on which the new value has to be displayed for line 2. Then, it changes to WRITE_CHAR2.
*	WRITE_CHAR2: Writes the new values to display (one value per LcdManager_UpdateTask call). When it finishes to write
*				 all the characters, the state machine return to IDLE.

**************************************************************************************************/

/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
#include "GPIO.h"
#include "derivative.h"
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/

//Pin configuration
#define LCD_CONTROL_BITS_MASK 0x0C
#define LCD_DATA_BITS_MASK 0xF0
#define D4_PIN 4
#define D5_PIN 5
#define D6_PIN 6
#define D7_PIN 7
#define ENABLE_PIN 2
#define RS_PIN 3

//States
#define INIT 0
#define IDLE 1
#define SET_POSITION1 2
#define WRITE_CHAR1 3
#define SET_POSITION2 4
#define WRITE_CHAR2 5

//Commands
#define EIGHT_BIT_CONFIG 0x03
#define FOUR_BIT_CONFIG 0x02
#define FUNCTION_SET 0x28
#define DISPLAY_CONTROL 0x0C
#define ENTRY_MODE 0x06
#define CLEAR_DISPLAY 0x01

//RS values
#define RS_CMD 0
#define RS_DATA 1

/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/
//Update the cursor position
void LcdManager_SetRequestedPosition(char u8Line);
//Commands for sending data
void LcdManager_ClearCmd(void);
void LcdManager_Send(char u8data, char u8forData);
//Pulses for enable signal
void LcdManager_PulseForClearCmd(void);
void LcdManager_200usEnablePulse(void);
//Send data to GPIO
void LcdManager_WriteDataPort(char u8data);
//MTIM delays
void LcdManager_5msDelay(void);
void LcdManager_100usDelay(void);
void LcdManager_20msDelay(void);

/*************************************************************************************************/
/*********************                  Static Variables                    **********************/
/*************************************************************************************************/
//Backed up parameters to write message
//Line 1--------------------
static char sgu8offset1;
static char * sgpu8String1; 
static char sgu8counter1;
//Line 2--------------------
static char sgu8offset2;
static char * sgpu8String2; 
static char sgu8counter2;

static char sgu8currentState = INIT;
static char sgu8LcdBusyFlag = 0;
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

void LcdManager_Init(void)
{	
	
	//Configure LCD data pins
	GPIO_ConfigPortOutputsByMask(PORTB,LCD_DATA_BITS_MASK);
	GPIO_ConfigPortOutputsByMask(PORTA,LCD_CONTROL_BITS_MASK);
	
	//Initial delay
	LcdManager_20msDelay();
	
	//4 bits configuration	
	LcdManager_WriteDataPort(EIGHT_BIT_CONFIG);
	LcdManager_200usEnablePulse();
	
	LcdManager_WriteDataPort(FOUR_BIT_CONFIG);
	LcdManager_200usEnablePulse();
	
	//Set 4 bit, 2 lines
	LcdManager_Send(FUNCTION_SET,RS_CMD);
	//Display on cursor off, first line
	LcdManager_Send(DISPLAY_CONTROL,RS_CMD);
	//Automatic Increment
	LcdManager_Send(ENTRY_MODE,RS_CMD);
	//Clear display
	LcdManager_ClearCmd(); 
	
	//Change to IDLE state
	sgu8currentState = IDLE;
}

void LcdManager_UpdateTask(void)
{
	if(sgu8currentState != INIT)
	{	
		switch(sgu8currentState)
		{
			case IDLE:
				if(sgu8LcdBusyFlag == 1)
				{
					sgu8currentState = SET_POSITION1;
				}
				break;
			case SET_POSITION1:
				sgu8currentState = WRITE_CHAR1;
				//Update cursor position for line 1
				LcdManager_SetRequestedPosition(1);
				break;
			case WRITE_CHAR1:
				LcdManager_Send(*sgpu8String1,RS_DATA);
				sgpu8String1++;
				sgu8counter1--;
				//Write one character for each call to this function
				if(sgu8counter1==0)
				{
					sgu8currentState = IDLE; //Return to IDLE state
					sgu8LcdBusyFlag = 0;
				}
				break;
			default:
				break;
		}
	}
}

void LcdManager_WriteString(char u8linea, char u8offset, char * pu8String, char u8length)
{
	sgu8LcdBusyFlag = 1; //Now, LCD display is busy
	
	//Back up parameters depending on the selected line
	if(u8linea == 1)
	{
		sgu8offset1 = u8offset;
		sgpu8String1 = pu8String; 
		sgu8counter1 = u8length;
	}
	else if(u8linea == 2)
	{
		sgu8offset2 = u8offset;
		sgpu8String2 = pu8String; 
		sgu8counter2 = u8length;
	}
}

char LcdManager_IsBusy(void)
{
	return sgu8LcdBusyFlag;
}	

/*************************************************************************************************/
/*********************				    Private Functions					**********************/
/*************************************************************************************************/
//Update the cursor position
void LcdManager_SetRequestedPosition(char u8Line)
{
	char u8LineMask;
	char u8OffsetMask;
	//Determine the corresponding mask depending on the line
	if(u8Line == 1)
	{
		u8LineMask = 0;
		u8OffsetMask = sgu8offset1;
	}
	else if(u8Line == 2)
	{
		u8LineMask = 0x40;
		u8OffsetMask = sgu8offset2;
	}
	//Change the cursor position
	LcdManager_Send(0x80|u8LineMask|u8OffsetMask,RS_CMD);
}

//Commands for sending data----------------------------------
void LcdManager_ClearCmd(void)
{
	char u8nibble_1 = (CLEAR_DISPLAY >> 4) & 0x0F;
	char u8nibble_2 = CLEAR_DISPLAY & 0x0F;
	GPIO_WriteBitToPin(PORTA, RS_PIN, RS_CMD);
	LcdManager_WriteDataPort(u8nibble_1); //Send nibble 1
	LcdManager_200usEnablePulse();
	LcdManager_WriteDataPort(u8nibble_2); //Send nibble 2
	LcdManager_PulseForClearCmd();
}

void LcdManager_Send(char u8data, char u8forData)
{
	char u8nibble_1 = (u8data >> 4) & 0x0F;
	char u8nibble_2 = u8data & 0x0F;
	GPIO_WriteBitToPin(PORTA, RS_PIN, u8forData);
	LcdManager_WriteDataPort(u8nibble_1); //Send nibble 1
	LcdManager_200usEnablePulse();
	LcdManager_WriteDataPort(u8nibble_2); //Send nibble 2
	LcdManager_200usEnablePulse();
	GPIO_WriteBitToPin(PORTA, RS_PIN, OFF);
}
//-----------------------------------------------------------

//Pulses for enable------------------------------------------
void LcdManager_PulseForClearCmd(void)
{
	GPIO_WriteBitToPin(PORTA, ENABLE_PIN, ON);
	LcdManager_100usDelay();
	GPIO_WriteBitToPin(PORTA, ENABLE_PIN, OFF);
	LcdManager_5msDelay();
}

void LcdManager_200usEnablePulse(void)
{
	GPIO_WriteBitToPin(PORTA, ENABLE_PIN, ON);
	LcdManager_100usDelay();
	GPIO_WriteBitToPin(PORTA, ENABLE_PIN, OFF);
	LcdManager_100usDelay();
}
//-----------------------------------------------------------

//Send data to GPIO
void LcdManager_WriteDataPort(char u8data)
{
	GPIO_WriteBitToPin(PORTB,D4_PIN,(u8data&0x01));
	GPIO_WriteBitToPin(PORTB,D5_PIN,(u8data>>1)&0x01);
	GPIO_WriteBitToPin(PORTB,D6_PIN,(u8data>>2)&0x01);
	GPIO_WriteBitToPin(PORTB,D7_PIN,(u8data>>3)&0x01);
}

//MTIM delays------------------------------------------------
void LcdManager_5msDelay(void)
{
	__asm{
		        LDX 50
		LOOP2: 	LDA 50
		LOOP1: 	STA SRS ;Reset WD
				DBNZA LOOP1
				DBNZX LOOP2
	}
}

void LcdManager_100usDelay(void)
{
	__asm{
			    LDA 50
		LOOP: 	STA SRS ;Reset WD
				DBNZA LOOP
	}
}

void LcdManager_20msDelay(void)
{
	__asm{
				LDX 200
		LOOP2: 	LDA 50
		LOOP1: 	STA SRS ;Reset WD
				DBNZA LOOP1
				DBNZX LOOP2
	}
}
