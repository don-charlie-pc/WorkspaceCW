/*! **********************************************************************************************
* 2014  ITESM Campus Guadalajara. Laboratorio de Microcontroladores 
*  
*
* @file:      ADC.c
* @author(s): 
* Abner Perez Haro A01224776
* Juan Carlos Perez Castellanos A01225629
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
#define EIGHT_MODE_MASK 0xF3;
#define TEN_MODE_MASK 0x08;
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
//ADC channel that is currently working
static char sgu8currentChannel = 0;
/*************************************************************************************************/
/*********************                  Static Constants                    **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************                  Global Constants                    **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************				   Exported Functions					**********************/
/*************************************************************************************************/
void ADC_Init(void)
{
	/*Configure ADC port and functions
	ADC channel 0 as default input
	8 bit conversion
	Compare function disable
	Interrupt disabled
	Input ADC Pin Control 0
	 */
	/*
	ADCCFG_ADLSMP is clear for short sample time
	ADCCFG_MODE is clear for 8 bit operation
	ADCCFG_ADICLK is clear to select (input clock source)/1
	 */
	ADCCFG = 0;
	
	/*
	ADCSC2_ADTRG is clear to select software trigger
	ADCSC2_ACFE is clear to disable compare function
	 */
	ADCSC2 = 0;
	
	/*
	ADC Control register. Clear all bits. 
	ADCSC1_AIEN is clear for Interruption disable;
	ADCSC1_ADCH is clear to select channel 0 (AD0)
	*/
    ADCSC1 = 0;
    
    /*
    APCTL1_ADPC0 is set to enable analog_input AD0
     */
    APCTL1 = sgu8currentChannel;
}
char ADC_Read_8(char * pu8destination)
{	
	//Check if conversion is finished
	if(ADCSC1_COCO)
	{
		//Stores the eight-bit conversion result in *pu8destination
		(*pu8destination) = ADCRL;
		return 1;
	}
	return 0;
}
char ADC_Read_10(char * pu8destination_H, char * pu8destination_L)
{	
	//Check if conversion is finished
	if(ADCSC1_COCO)
	{
		//Stores the ten-bit conversion result in *pu8destination_H/L.
		(*pu8destination_H) = ADCRH;
		(*pu8destination_L) = ADCRL;
	}
	return ADCSC1_COCO;
}
void ADC_StartConversion(void)
{
	//Writes a value to ADCSC1 to start a new conversion
	ADCSC1 = sgu8currentChannel;
}
void ADC_ConfigResolution(char u8eightModeEnable)
{
	//Set the eight-bit mode conversion
	if(u8eightModeEnable)
	{
		ADCCFG &= EIGHT_MODE_MASK;
	}
	else
	{
		//Set the ten-bit mode conversion
		ADCCFG |= TEN_MODE_MASK;
	}
}
void ADC_selectChannel(char u8channel)
{
	//Set ADC channel to read, from ADC0 to ADC27
	if(0 <= u8channel && 27 >= u8channel)
	{
		sgu8currentChannel = u8channel;
	}
}
/*************************************************************************************************/
/*********************				    Private Functions					**********************/

/*************************************************************************************************/

