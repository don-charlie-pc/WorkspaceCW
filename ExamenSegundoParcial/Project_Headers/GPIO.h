/*! **********************************************************************************************
* 2014  ITESM Campus Guadalajara. Laboratorio de Microcontroladores 
*
*
* @file:      GPIO.h
* @author(s):  
* Abner Perez Haro
* Juan Carlos Perez Castellanos
*
* @brief (User's Manual)

**************************************************************************************************/

#ifndef GPIO_H_
#define GPIO_H_

 
/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
#define PORTA 0
#define PORTB 1
#define INPUT 0
#define OUTPUT 1
#define DIGITAL 0
#define ANALOG 1
#define OFF 0
#define ON 1
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
/*************************************************************************************************/
void GPIO_ConfigPinDirection(char u8port, char u8pin, char u8dir);
/*
	Configure an specific pin of a PORT as input or output.
	Parameters
	u8dir:
		0 Input
		1 Output
	u8port: 
		0 PTAD
		1 PTBD
	u8pin: from 0 to 7.
*/
void GPIO_ConfigPortDirection(char u8port, char u8dir);
/*
	Configure the all pins of a PORT as input or output.
	Parameters
	u8dir:
		0 Input
		1 Output
	u8port: 
		0 PTAD
		1 PTBD
*/
void GPIO_ConfigPortDirection_Mask(char u8port, char u8mask);
/*
	Apply an specific mask (u8mask) to a PORT, to configure certain pins as inputs and others as outputs.
	If a bit of the mask is 0, the pin will be input; if 1, output.
	Parameters
	u8port: 
		0 PTAD
		1 PTBD
*/
char GPIO_ReadPort(char u8port);
/*
	Read the complete byte from the port indicated by u8port.
	Parameters
	u8port: 
		0 PTAD
		1 PTBD
*/
char GPIO_ReadPin(char u8port,char u8pin);
/*
	Read certain pin from the port indicated by u8port.
	Parameters
	u8port: 
		0 PTAD
		1 PTBD
	u8pin: from 0 to 7.
*/
void GPIO_WriteBitToPin(char u8port, char u8pin, char u8value);
/*
	Write certain pin of the port indicated by u8port.
	Parameters
	u8port: 
		0 PTAD
		1 PTBD
	u8pin: from 0 to 7.
	u8value: 0 or 1
*/
void GPIO_WriteByteToPort(char u8port, char u8value);
/*
	Write a complete byte (u8value) in the port indicated by u8port.
	Parameters
	u8port: 
		0 PTAD
		1 PTBD
*/
void GPIO_ConfigPortInputsByMask(char u8port,char u8mask);
void GPIO_ConfigPortOutputsByMask(char u8port, char u8mask);
void GPIO_SetPortPinsByMask(char u8port, char u8mask);
void GPIO_ClearPortPinsByMask(char u8port, char u8mask);
/*************************************************************************************************/
#endif /* GPIO_H_ */
