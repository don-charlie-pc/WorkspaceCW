/*! **********************************************************************************************
* 2014  ITESM Campus Guadalajara. Laboratorio de Microcontroladores 
*
*
* @file:      TPM.h
* @author(s):  
* Abner Perez Haro A01224776
* Juan Carlos Perez Castellanos A01225629
* 
* @brief (User's Manual)
**************************************************************************************************/

#ifndef TPM_H_
#define TPM_H_

 
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
void TPM_Init(void);
/*
		Reset TPM module and initialise as default configuration: 
		*Enable flag interrupt
		*No clock source selected
		*Prescale divisor 1/1
*/

void TPM_SetExtraFlags(char u8numFlags);
/*
	Set the number of times timer interrupt flag has to be triggered before the timer stops.
*/
void TPM_SetModuloVal(unsigned int u16value);
/*
	Configure timer modulo.
*/
void TPM_Start(void);
/*
	Start timer operation. It will stop when the maximum number of flags is reached.    
*/
void TPM_Stop(void);
/*
	Stop timer operation.
*/
char TPM_IsRunning(void);
/*
	It returns 1 if timer is running, and 0 if it is stopped.
*/

void TPM_EnableInputCaptureCH0(void);
//Enable input capture for channel 0

void TPM_EnableInputCaptureCH1(void);
//Enable input capture for channel 1

void TPM_StopOnOverflow(char u8stop);
//Configure TPM to stop or to continue after a interrupt flag

unsigned int TPM_getCH0Val(void);
//Get value from CH0

unsigned int TPM_getCH1Val(void);
//Get value from CH1

void TPM_Reset(void);
//Reset TPM count

void TPM_SetPrescaler(char u8prescaler);
/*
 	Change PRESCALER:
 	0: 1
 	1: 2
 	2: 4
 	3: 8
 	4: 16
 	5: 32
 	6: 64
 	7: 128
 */

void TPM_ResetStepCounter(void);
//Reset motor step counter

char TPM_GetStep(void);
//Get current step (from 0 to 47)

void TPM_SetDir(char u8dir);
//Set step counter to increment or decrement
/*************************************************************************************************/
#endif /* TPM_H_ */
