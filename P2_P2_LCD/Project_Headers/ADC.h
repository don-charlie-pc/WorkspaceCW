/*! **********************************************************************************************
* 2014  ITESM Campus Guadalajara. Laboratorio de Microcontroladores 
*
*
* @file:      ADC.h
* @author(s):  
* Abner Perez Haro A01224776
* Juan Carlos Perez Castellanos A01225629
*
* @brief (User's Manual)
**************************************************************************************************/

#ifndef ADC_H_
#define ADC_H_

 
/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************			 	 Functions Like Macros					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Extern Variables					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************                  Extern Constants                    **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/

void ADC_Init(void);
/*
	Initialise the ADC configuration as default: 
	-Channel 0 is selected, eight conversion, compare function disable and interrupt disable.
*/
char ADC_Read_8(char * pu8destination);
/*
	Try to read the ADC selected channel. It returns 1 if the conversion is ready, otherwise returns 0.
	This function stores the eight-bit conversion read value in the content of pu8destiny pointer.
*/
char ADC_Read_10(char * pu8destination_H, char * pu8destination_L);
/*
	Try to read the ADC selected channel. It returns 1 if the conversion is ready, otherwise returns 0.
	This function stores the ten-bit conversion read value in the content of pu8destination_H and pu8destination_L pointer.
*/
void ADC_StartConversion(void);
/*
	It starts a new conversion.
*/
void ADC_ConfigResolution(char u8eightModeEnable);
/*
	Select the resolution mode. If u8eightModeEnable is 0, ten-bit mode is selected, otherwise eight-bit mode. 
*/
void ADC_selectChannel(char u8channel);
/*
	Select the ADC channel from 0 to 27.
*/

/*************************************************************************************************/

/*************************************************************************************************/
#endif /* ADC_H_ */
