/*! **********************************************************************************************
* 2014  ITESM Campus Guadalajara. Laboratorio de Microcontroladores 
*  
*
* @file:      SpeedManager.c
* @author(s):  
* Abner Perez Haro A01224776
* Juan Carlos Perez Castellanos A01225629
* 
* @brief (Theory of Operation)
* This driver gets the ADC value, and though this one calculates the speed level.
* From 0 (0.2 rev/sec) to 28 level (5.5 rev/sec).  

**************************************************************************************************/


/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
#include "ADC.h"
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
#define SPEED_LIMIT 28
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

void SpeedManager_Init(void)
{
	//Initialise ADC controller
	ADC_Init();	
}

char SpeedManager_ReadSpeed(void)
{
	char u8AdcValue = 0;
	char u8speed = 0;
	
	ADC_StartConversion();
	//Wait until ADC conversion ends
	while(!ADC_Read_8(&u8AdcValue)); 
	
	//Divide over four ADC value to get the level speed
	u8speed = (u8AdcValue >> 2)&0x3F;
	
	//Protection in case ADC reading were greater than the speed limit
	if(u8speed > SPEED_LIMIT)
	{
		u8speed = SPEED_LIMIT;
	}
	
	return u8speed;
}

/*************************************************************************************************/
/*********************				    Private Functions					**********************/
/*************************************************************************************************/
