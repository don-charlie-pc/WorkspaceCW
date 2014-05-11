/*! **********************************************************************************************
* 2014  ITESM Campus Guadalajara. Laboratorio de Microcontroladores 
*  
*
* @file:      SerialManager.c
* @author(s):  
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
#define BPS_OVER_100_MAX 1152
#define BPS_OVER_100_MIN 96
#define DEFAULT_BR 26
#define SCI_CONFIG 0x2C
/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************                  Static Variables                    **********************/
/*************************************************************************************************/
static unsigned char sgu8data; 
static unsigned char sgu8received;
static unsigned char sgu8complete;
static unsigned char sgu8full;
static unsigned char sgu8byteToSend;
static unsigned char finish_program = 0;
static unsigned char counter = 0;
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
void SerialManager_Init(void)
{
	//Configure default baud rate 9600 bps
	SCIBD = DEFAULT_BR;
	//Enable reception and transmission interrupt
	SCIC2 = SCI_CONFIG;
	sgu8received = 0;
	sgu8complete = 0;
	sgu8full = 0;
	sgu8byteToSend = 0;
}

void SerialManager_SetBaudRate(unsigned int u16BpsOver100)
{
	if(u16BpsOver100 >= BPS_OVER_100_MIN && u16BpsOver100 <= BPS_OVER_100_MAX)
	{
		SCIBD = 2500/u16BpsOver100;
	}
}

void SerialManager_GetValue(unsigned char *n_data, unsigned char *u8data, unsigned char *complete, unsigned char *finished)
{
	if(counter == 10)
	{
		finish_program = 1;
	}
	(*n_data) =  sgu8received;
	(*u8data) = sgu8byteToSend;
	(*complete) = sgu8complete;
	//after send the data, sgu8received is 0
	sgu8received = 0;
	(*finished) = finish_program;
}
/*************************************************************************************************/
/*********************				    Private Functions					**********************/
/*************************************************************************************************/
unsigned char char_value(unsigned char nibble)
{
	//When 0 - 9
	if((nibble <= 0x39) && (nibble >= 0x30))
	{
		return (nibble - 0x30);
	}
	else	// When A - F
	{
		return (nibble-55);
	}
}
void check_finish(void)
{
	if((char_value(sgu8data)) == 0x03)
	{
		counter++;
	}
	else{
		counter = 0;
	}
}

interrupt 15 void SCIreceive(void)
{
	//Clear receiver flag
	(void) SCIS1;
	//Get character
	sgu8data = SCID;
	//si se completa el segundo nibble, sgu8complete = true;
	//If high nibble
	if(sgu8full == 0)
	{
		sgu8complete = 0;
		sgu8full = 1;
		sgu8byteToSend = (char_value(sgu8data) << 4);
		sgu8byteToSend &= 0xF0;	
	}
	//If low nibble
	else // if(sgu8full == 1)
	{
		sgu8complete = 1;
		sgu8full = 0;
		sgu8byteToSend |= char_value(sgu8data);
	}
	//sgu8received indicates if there is new data
	sgu8received = 1;
	check_finish();
	__RESET_WATCHDOG();
}
