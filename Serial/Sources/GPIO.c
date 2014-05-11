/*! **********************************************************************************************
* 2014  ITESM Campus Guadalajara. Laboratorio de Microcontroladores 
*  
*
* @file:      GPIO.c
* 
* @author(s):  
* Abner Perez Haro
* Juan Carlos Perez Castellanos
*
* @brief (Theory of Operation)
* It controls the port direction (input-output), and allows to read and write from complete ports or
* certain pins.

**************************************************************************************************/


/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
#include "derivative.h"
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
#define SET_BIT_MASK 0x01
#define CLEAR_BIT_MASK 0x80
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
static char u8PTADD = 0x00;  	//PTADD Backup 
static char u8PTBDD = 0x00;  	//PTBDD Backup
static char u8PTAD = 0x00;  	//PTADD Backup 
static char u8PTBD = 0x00;  	//PTBDD Backup

/*************************************************************************************************/
/*********************                  Static Constants                    **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************                  Global Constants                    **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************				   Exported Functions					**********************/
/*************************************************************************************************/
void GPIO_ConfigPinDirection(char u8port, char u8pin, char u8dir)
{
	char u8mask = 0;
	if(u8port)	
	//Configure pin direction in PTBD
	{
		if(u8dir) //Set bit as output
		{
			u8mask = SET_BIT_MASK << u8pin;
			u8PTBDD |= u8mask;
			PTBDD = u8PTBDD;
		}
		else	//Set bit as input
		{
			u8mask = ~(CLEAR_BIT_MASK >> (7 - u8pin));
			u8PTBDD &= u8mask;
			PTBDD = u8PTBDD;
		}
	}
	else		
	//Configure pin direction in PTAD
	{
		if(u8dir) 
		//Set bit as output
		{
			u8mask = SET_BIT_MASK << u8pin;
			u8PTADD |= u8mask;
			PTADD = u8PTADD;
		}
		else	
		//Set bit as input
		{
			u8mask = ~(CLEAR_BIT_MASK >> (7 - u8pin));
			u8PTADD &= u8mask;
			PTADD = u8PTADD;
		}
	}
}
void GPIO_ConfigPortDirection(char u8port, char u8dir)
{
	if(u8port) 	
	//Configure PTBD direction
	{
		if(u8dir) 
		//PTBD is Output
		{
			u8PTBDD = 0xFF;
		}
		else	
		//PTBD is Input
		{
			u8PTBDD = 0x00;
		}
		PTBDD = u8PTBDD;
	}
	else 		
	//Configure PTAD direction
	{
		if(u8dir) 
		//PTAD is Output
		{
			u8PTADD = 0xFF;
		}
		else	
		//PTAD is Input
		{
			u8PTADD = 0x00;
		}
		PTADD = u8PTADD;
	}
}
void GPIO_ConfigPortDirection_Mask(char u8port, char u8mask)
{
	if(u8port)	
	//Set mask to PTBDD
	{
		u8PTBDD = u8mask;
		PTBDD = u8PTBDD;
	}
	else 		
	//Set mask to PTADD
	{
		u8PTADD = u8mask;
		PTADD = u8PTADD;
	}
}
char GPIO_ReadPort(char u8port)
{
	//Backup of PTAD and PTBD values
	u8PTAD = PTAD;
	u8PTBD = PTBD;
	if(u8port) 
	{		
		//Returns PTBD value
		return u8PTBD;
	}
	// Returns PTAD value
	return u8PTAD;
}
char GPIO_ReadPin(char u8port,char u8pin)
{
	char u8pin_value = 0;	//Value of a specific pin, 0 or 1
	//Backup of PTAD and PTBD values
	u8PTAD = PTAD;
	u8PTBD = PTBD;
	if(u8port) //Reads pin in PTBD
	{
		u8pin_value = u8PTBD;
	}
	else	 //Reads ping in PTAD
	{
		u8pin_value = u8PTAD;
	}
	//Right shift (u8pin times) u8pin_value to store the pin value at the pin0
	u8pin_value = u8pin_value >> u8pin;
	//Apply mask to keep only pin0 value
	u8pin_value &= 0x01;
	//Returns value (1 or 0) of an specific PTAD/BD pin
	return u8pin_value;
}
void GPIO_WriteBitToPin(char u8port, char u8pin, char u8value)
{
	char u8mask = 0;
	//Backup of PTAD and PTBD values
	u8PTAD = PTAD;
	u8PTBD = PTBD;
	if(u8port)		//PTBD
	{
		if(u8value) 	
		//Value is 1. Set bit in PTBD
		{
			//Left shifting (u8pin times) to set mask for the specific pin
			u8mask = SET_BIT_MASK << u8pin;
			u8PTBD |= u8mask;
			PTBD = u8PTBD;
		}
		else		
		//Value is 0. Clear bit in PTBD
		{
			//Left shifting (u8pin times) to set mask for the specific pin
			u8mask = ~(CLEAR_BIT_MASK >> (7 - u8pin));
			u8PTBD &= u8mask;
			PTBD = u8PTBD;
		}
	}
	else			//PTAD
	{
		if(u8value) 	//Value is 1. Set bit in PTAD
		{
			//Left shifting (u8pin times) to set mask for the specific pin
			u8mask = SET_BIT_MASK << u8pin;
			u8PTAD |= u8mask;
			PTAD = u8PTAD;
		}
		else		//Value is 0. Clear bit in PTAD
		{
			//Left shifting (u8pin times) to set mask for the specific pin
			u8mask = ~(CLEAR_BIT_MASK >> (7 - u8pin));
			u8PTAD &= u8mask;
			PTAD = u8PTAD;
		}
	}
}
void GPIO_WriteByteToPort(char u8port, char u8value)
{
	if(u8port)	//Write u8value to PTBD
	{
		u8PTBD = u8value;
		PTBD = u8PTBD;
	}
	else 		//Write u8value to PTAD
	{
		u8PTAD = u8value;
		PTAD = u8PTAD;
	}
}
void GPIO_ConfigPortInputsByMask(char u8port,char u8mask)
{
	if(u8port) //PTBDD selected
	{
		u8PTBDD &= (!u8mask);
		PTBDD = u8PTBDD;
	}
	else //PTADD selected
	{
		u8PTADD &= (!u8mask);
		PTADD = u8PTADD;
	}
}
void GPIO_ClearPortPinsByMask(char u8port, char u8mask)
{
	if(u8port) //PTBD selected
	{
		u8PTBD &= (!u8mask);
		PTBD = u8PTBD;
	}
	else //PTAD selected
	{
		u8PTAD &= (!u8mask);
		PTAD = u8PTAD;
	}
}
void GPIO_ConfigPortOutputsByMask(char u8port, char u8mask)
{
	if(u8port) //PTBDD selected
	{
		u8PTBDD |= u8mask;
		PTBDD = u8PTBDD;
	}
	else //PTADD selected
	{
		u8PTADD |= u8mask;
		PTADD = u8PTADD;
	}
}
void GPIO_SetPortPinsByMask(char u8port, char u8mask)
{
		if(u8port) //PTBD selected
	{
		u8PTBD |= u8mask;
		PTBD = u8PTBD;
	}
	else //PTAD selected
	{
		u8PTAD |= u8mask;
		PTAD = u8PTAD;
	}
}


/*************************************************************************************************/
/*********************				    Private Functions					**********************/
/*************************************************************************************************/

