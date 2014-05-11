/*! **********************************************************************************************
* 2014  ITESM Campus Guadalajara. Laboratorio de Microcontroladores 
*  
*
* @file:      KeyboardManager.c
* @author(s):  
*
* @brief (Theory of Operation)
*
**************************************************************************************************/


/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
#include "GPIO.h"
#include <hidef.h>
#include "derivative.h"
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
//Configuration mask
#define KB_ROW_MASK 0x0F
#define KB_COL_MASK 0x0F
#define KB_CONFIG_MASK 0x02
#define KB_ENABLE_PIN_MASK 0x0F
#define KB_SET_ACK 0x04

//Pins
#define R0 0
#define R1 1
#define R2 2
#define R3 3
/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/
void KeyboardManager_WriteRows(char u8rowsValue);
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
unsigned char gu8numKey;
/*************************************************************************************************/
/*********************				   Exported Functions					**********************/
/*************************************************************************************************/

void KeyboardManager_Init(void)
{
	//Use PTAD[0:3] as rows and PTBD[0:3] as columns
	GPIO_ConfigPortOutputsByMask(PORTB,KB_ROW_MASK);
	GPIO_ConfigPortInputsByMask(PORTA,KB_COL_MASK);
	
	//Enable keyboard interrupt
	KBISC = KB_CONFIG_MASK;
	
	//Enable PTAD[0:3] for interrupt
	KBIPE = KB_ENABLE_PIN_MASK;
	
	//Set rows
	KeyboardManager_WriteRows(0);
}


/*************************************************************************************************/
/*********************				    Private Functions					**********************/
/*************************************************************************************************/
interrupt 18 void KeyboardManager_ISR(void)
{
	char u8scanner = 0xFE;
	char u8count = 0;
	unsigned char u8ColsValue;
	char u8keyDetected = 0;
	char u8numRow = 0;
	char u8numCol = 0;
	
	//Clear flag by setting ACK bit
	KBISC = KB_SET_ACK;
	//DisableInterrupts;
	
	
	u8ColsValue = GPIO_ReadPort(PORTA)&0x0F;
	
	if(u8ColsValue&0x0E)
	{
		u8numCol = 0;
	}
	else if(u8ColsValue&0x0D)
	{
		u8numCol = 1;
	}
	else if(u8ColsValue&0x0B)
	{
		u8numCol = 2;
	}
	else if(u8ColsValue&0x07)
	{
		u8numCol = 3;
	}
	
	do
	{
		KeyboardManager_WriteRows(u8scanner);
		u8ColsValue = GPIO_ReadPort(PORTA)&0x0F;
		u8keyDetected = !(u8ColsValue==0x0F);
		u8scanner = (u8scanner << 1)|0x01;
		u8count++;
	}while(u8count<4 && !u8keyDetected);
	
	if(u8keyDetected)
	{
		if(u8ColsValue&0x0E)
		{
			u8numRow = 0;
		}
		else if(u8ColsValue&0x0D)
		{
			u8numRow = 1;
		}
		else if(u8ColsValue&0x0B)
		{
			u8numRow = 2;
		}
		else if(u8ColsValue&0x07)
		{
			u8numRow = 3;
		}
		gu8numKey = 4*u8numRow+u8numCol;
	}
	
	EnableInterrupts;
}

void KeyboardManager_WriteRows(char u8rowsValue)
{
	GPIO_WriteBitToPin(PORTB,R0,(u8rowsValue&0x01));
	GPIO_WriteBitToPin(PORTB,R1,(u8rowsValue>>1)&0x01);
	GPIO_WriteBitToPin(PORTB,R2,(u8rowsValue>>2)&0x01);
	GPIO_WriteBitToPin(PORTB,R3,(u8rowsValue>>3)&0x01);
}
