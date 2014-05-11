/*! **********************************************************************************************
* 2014  ITESM Campus Guadalajara. Laboratorio de Microcontroladores 
*
*
* @file:      LcdManager.h
* @author(s):  
*
* @brief (User's Manual)
*
**************************************************************************************************/

#ifndef LCDMANAGER_H_
#define LCDMANAGER_H_

 
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
/*************************************************************************************************/
char LcdManager_IsBusy(void);
void LcdManager_Init(void);
/*
	Initialise the LCD with default configuration:
	* MTIM to make the delays when sending data or commands
	* Data pins in Port B [7..4]
	* Control pins in Port A: Enable is Pin2, RS is Pin3
	* 4 bits LCD mode, 2 lines
	* Display on cursor off, automatic increment
 */
void LcdManager_UpdateTask(void);
/*
	Updates the state machine for the LCD. The states are:
	*INIT: Initial state, after LCD is initialised.
	*IDLE: State when there are no updated values and LCD is not printing (Busy flag is clear). 
	       Until the BusyFlag is set (new values to display), it changes to SET_POSITION state. 
	*SET_POSITION: Set the position on which the new value has to be displayed. Then changes to WRITE_CHAR
	*WRITE_CHAR: Writes all the new values to display (one value per cycle). 
				 The state changes to IDLE when all the values were written to the LCD.	
 */
void LcdManager_WriteString(char u8linea, char u8offset, char * pu8String, char u8length);
/*
	Set the LCD BusyFlag.
	Updates values of line, offset, a string containing an specific value, and length of string.
 */

/*************************************************************************************************/
#endif /* LCDMANAGER_H_ */
