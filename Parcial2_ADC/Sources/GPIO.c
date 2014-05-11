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
*
**************************************************************************************************/


/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
#include "derivative.h"
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
char u8set = 0x01; //Set bit
char u8clear = 0xFE;  //Clear bit
char u8PTADD = 0x00;  //PTADD Backup 
char u8PTBDD = 0x00;  //PTBDD Backup
char u8PTAD = 0x00;  //PTADD Backup 
char u8PTBD = 0x00;  //PTBDD Backup
char u8mask = 0x00;	  //Mask
char u8pin_value = 0;		  //Valor de pin en u8pin_value[0]
/*************************************************************************************************/
/*********************                  Static Constants                    **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************                  Global Constants                    **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************				   Exported Functions					**********************/
/*************************************************************************************************/
void GPIO_ConfigPinDirection(char port, char pin, char dir)
{
	//port = 0 is PTADD
	//port = 1 is PTBDD
	if(port)	//PTBDD
	{
		if(dir) //Set bit output
		{
			u8mask = u8set;
			u8mask = u8mask << pin;
			u8PTBDD |= u8mask;
			PTBDD = u8PTBDD;
		}
		else	//Set bit input
		{
			u8mask = u8clear;
			u8mask = u8mask << pin;
			u8PTBDD &= u8mask;
			PTBDD = u8PTBDD;
		}
	}
	else		//PTADD
	{
		if(dir) //Set bit output
		{
			u8mask = u8set;
			u8mask = u8mask << pin;
			u8PTADD |= u8mask;
			PTADD = u8PTADD;
		}
		else	//Set bit input
		{
			u8mask = u8clear;
			u8mask = u8mask << pin;
			u8PTADD &= u8mask;
			PTADD = u8PTADD;
		}
	}
	u8mask = 0x00;	//Clear u8mask
}
void GPIO_ConfigPortDirection(char port, char dir)
{
	if(port) 	//PTBDD
	{
		if(dir) //PTBD is Output
		{
			u8PTBDD = 0xFF;
		}
		else	//PTBD is Input
		{
			u8PTBDD = 0x00;
		}
		PTBDD = u8PTBDD;
	}
	else 		//PTADD
	{
		if(dir) //PTAD is Output
		{
			u8PTADD = 0xFF;
		}
		else	//PTAD is Input
		{
			u8PTADD = 0x00;
		}
		PTADD = u8PTADD;
	}
}
void GPIO_ConfigPortDirection_Mask(char port, char mask)
{
	if(port)	//PTBDD
	{
		u8PTBDD = mask;
		PTBDD = u8PTBDD;
	}
	else 		//PTADD
	{
		u8PTADD = mask;
		PTADD = u8PTADD;
	}
}
char GPIO_ReadPort(char port)
{
	u8PTAD = PTAD;
	u8PTBD = PTBD;
	if(port) //PTBD
	{		
		return u8PTBD;
	}
	// PTAD
	return u8PTAD;
}
char GPIO_ReadPin(char port,char pin)
{
	u8PTAD = PTAD;
	u8PTBD = PTBD;
	if(port) //Read PTBD
	{
		u8pin_value = u8PTBD;
	}
	else	 //Read PTAD
	{
		u8pin_value = u8PTAD;
	}
	u8pin_value = u8pin_value >> pin;
	u8pin_value &= 0x01;
	return u8pin_value;
}
void GPIO_WritePin(char port, char pin, char value)
{
	//0 is PTAD
	//1 is PTBD
	u8PTAD = PTAD;
	u8PTBD = PTBD;
	if(port)		//PTBD
	{
		if(value) 	//Value is 1. Set bit
		{
			u8mask = u8set;
			u8mask = u8mask << pin;
			u8PTBD |= u8mask;
			PTBD = u8PTBD;
		}
		else		//Value is 0. Clear bit
		{
			u8mask = u8clear;
			u8mask = u8mask << pin;
			u8PTBD &= u8mask;
			PTBD = u8PTBD;
		}
	}
	else			//PTAD
	{
		if(value) 	//Value is 1. Set bit
		{
			u8mask = u8set;
			u8mask = u8mask << pin;
			u8PTAD |= u8mask;
			PTAD = u8PTAD;
		}
		else		//Value is 0. Clear bit
		{
			u8mask = u8clear;
			u8mask = u8mask << pin;
			u8PTAD &= u8mask;
			PTAD = u8PTAD;
		}
	}
	u8mask = 0x00;
}
void GPIO_WritePort(char port, char value)
{
	if(port)	//PTBD
	{
		u8PTBD = value;
		PTBD = u8PTBD;
	}
	else 		//PTAD
	{
		u8PTAD = value;
		PTAD = u8PTAD;
	}
}

/*************************************************************************************************/
/*********************				    Private Functions					**********************/
/*************************************************************************************************/

