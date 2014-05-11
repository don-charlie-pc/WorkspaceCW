/*! **********************************************************************************************
* 2014  ITESM Campus Guadalajara. Laboratorio de Microcontroladores 
*  
*
* @file:      ADC.c
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
#define eightModeMask 0xF3;
#define tenModeMask 0x08;
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
char u8currentChannel = 0;
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
	/*Default configuration
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
    APCTL1 = u8currentChannel;
}
int ADC_Read_8(char * destination)
{	
	if(ADCSC1_COCO)
	{
		(*destination) = ADCRL;
	}
	return ADCSC1_COCO;
}
int ADC_Read_10(char * destination_H, char * destination_L)
{	
	if(ADCSC1_COCO)
	{
		(*destination_H) = ADCRH;
		(*destination_L) = ADCRL;
	}
	return ADCSC1_COCO;
}
void ADC_StartConversion(void)
{
	ADCSC1 = u8currentChannel;
}
void ADC_ConfigResolution(char eightModeEnable)
{
	if(eightModeEnable)
	{
		ADCCFG &= eightModeMask;
	}
	else
	{
		ADCCFG |= tenModeMask;
	}
}
void ADC_selectChannel(char channel)
{
	if(0 <= channel && 27 >= channel)
	{
		u8currentChannel = channel;
	}
}
/*************************************************************************************************/
/*********************				    Private Functions					**********************/

/*************************************************************************************************/
