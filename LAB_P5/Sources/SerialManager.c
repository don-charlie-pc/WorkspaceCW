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

#define IDLE_R 0
#define DIR_ID1 1
#define DIR_ID2 2
#define DIR_ID3 3
#define DIR_ID4 4
#define DIR_PAR1 5
#define DIR_PAR2 6
#define DIR_PAR3 7

#define M_ID1 8
#define M_ID2 9
#define M_ID3 10
#define M_ID4 11
#define M_ID5 12
#define M_ID6 13
#define M_PAR1 14
#define M_PAR2 15

#define RPM_ID1 16
#define RPM_ID2 17
#define RPM_ID3 18
#define RPM_ID4 19
#define RPM_PAR1 20
#define RPM_PAR2 21
#define RPM_PAR3 22
#define RPM_PAR4 23
#define RPM_PAR5 24

#define IDLE_T 0
#define WRITE_CHAR 1


/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/
void SerialManager_UpdateStateMachine(char u8data);
void SerialManager_Send(char u8data);
/*************************************************************************************************/
/*********************                  Static Variables                    **********************/
/*************************************************************************************************/
static unsigned char sgu8stateR;
static unsigned char sgu8stateT;
static unsigned char sgu8dir;
static unsigned char sgu8isOn;
static unsigned char sgu8RpmDigits [3];
static unsigned int sgu16RpmSpeed;
static unsigned char sgu8data; 

static char * sgpu8String; 
static char sgu8counter;

static char sgu8BusyFlag = 0;
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
	
	//Initialise State Machine
	sgu8stateR = IDLE_R;
	sgu8stateT = IDLE_T;
	
	//Set minimum speed
	sgu16RpmSpeed = 110;
	
	sgu8BusyFlag = 0;
}

void SerialManager_SetBaudRate(unsigned int u16BpsOver100)
{
	if(u16BpsOver100 >= BPS_OVER_100_MIN && u16BpsOver100 <= BPS_OVER_100_MAX)
	{
		SCIBD = 2500/u16BpsOver100;
	}
}

void SerialManager_GetValues(char *dir, char *isOn, unsigned int *speed)
{	
    (*dir) = sgu8dir;
    (*isOn) = sgu8isOn;
    (*speed) = sgu16RpmSpeed;
}

void SerialManager_WriteString(char * pu8String, char u8length)
{
	sgu8BusyFlag = 1;
	sgpu8String = pu8String; 
	sgu8counter = u8length;
}

void SerialManager_UpdateTask(void)
{
	switch(sgu8stateT)
	{
		case IDLE_T:
			if(sgu8BusyFlag == 1)
			{
				sgu8stateT = WRITE_CHAR;
			}
			break;
		case WRITE_CHAR:
			SerialManager_Send(*sgpu8String);
			sgpu8String++;
			sgu8counter--;
			//Write one character for each call to this function
			if(sgu8counter==0)
			{
				sgu8stateT = IDLE_T;
				sgu8BusyFlag = 0;
			}
			break;
		default:
			break;
	}
}

char SerialManager_IsBusy(void)
{
	return sgu8BusyFlag;
}	

/*************************************************************************************************/
/*********************				    Private Functions					**********************/
/*************************************************************************************************/
interrupt 15 void SCIreceive(void)
{
	//Clear receiver flag
	(void) SCIS1;
	//Get character
	sgu8data = SCID;
	SerialManager_UpdateStateMachine(sgu8data);
	__RESET_WATCHDOG();
}

void SerialManager_Send(char u8data)
{
	while(!SCIS1_TDRE);
	SCID = u8data;
}

void SerialManager_UpdateStateMachine(char u8data)
{
	switch(sgu8stateR)
	{
		case IDLE_R:
			if('D' == u8data)
			{
				sgu8stateR = DIR_ID1;
			}
			else if('M' == u8data)
			{
				sgu8stateR = M_ID1;
			}
			else if('R' == u8data)
			{
				sgu8stateR = RPM_ID1;
			}
			break;
		
		//Direction Command------------------------------
		case DIR_ID1:
			if('I' == u8data)
			{
				sgu8stateR = DIR_ID2;
			}
			else
			{
				sgu8stateR = IDLE_R;
			}
			break;
		case DIR_ID2:
			if('R' == u8data)
			{
				sgu8stateR = DIR_ID3;
			}
			else
			{
				sgu8stateR = IDLE_R;
			}
			break;
		case DIR_ID3:
			if(':' == u8data)
			{
				sgu8stateR = DIR_ID4;
			}
			else
			{
				sgu8stateR = IDLE_R;
			}
			break;
		case DIR_ID4:
			if('C' == u8data)
			{
				sgu8stateR = DIR_PAR1;
			}
			else
			{
				sgu8stateR = IDLE_R;
			}
			break;
		case DIR_PAR1:
			if('W' == u8data)
			{
				sgu8dir = 1;
				sgu8stateR = IDLE_R;
			}
			else if('C' == u8data)
			{
				sgu8stateR = DIR_PAR2;
			}
			else
			{
				sgu8stateR = IDLE_R;
			}
			break;
		case DIR_PAR2:
			if('W' == u8data)
			{
				sgu8dir = 0;
			}
			sgu8stateR = IDLE_R;
			break;
		
		//Motor On Off Command------------------------------
		case M_ID1:
			if('O' == u8data)
			{
				sgu8stateR = M_ID2;
			}
			else
			{
				sgu8stateR = IDLE_R;
			}
			break;
		case M_ID2:
			if('T' == u8data)
			{
				sgu8stateR = M_ID3;
			}
			else
			{
				sgu8stateR = IDLE_R;
			}
			break;
		case M_ID3:
			if('O' == u8data)
			{
				sgu8stateR = M_ID4;
			}
			else
			{
				sgu8stateR = IDLE_R;
			}
			break;
		case M_ID4:
			if('R' == u8data)
			{
				sgu8stateR = M_ID5;
			}
			else
			{
				sgu8stateR = IDLE_R;
			}
			break;
		case M_ID5:
			if(':' == u8data)
			{
				sgu8stateR = M_ID6;
			}
			else
			{
				sgu8stateR = IDLE_R;
			}
			break;
		case M_ID6:
			if('O' == u8data)
			{
				sgu8stateR = M_PAR1;
			}
			else
			{
				sgu8stateR = IDLE_R;
			}
			break;
		case M_PAR1:
			if('N' == u8data)
			{
				sgu8isOn = 1;
				sgu8stateR = IDLE_R;
			}
			else if('F' == u8data)
			{
				sgu8stateR = M_PAR2;
			}
			else
			{
				sgu8stateR = IDLE_R;
			}
			break;
		case M_PAR2:
			if('F' == u8data)
			{
				sgu8isOn = 0;
			}
				sgu8stateR = IDLE_R;
			break;
	
		
		//RPM Command------------------------------
		case RPM_ID1:
			if('P' == u8data)
			{
				sgu8stateR = RPM_ID2;
			}
			else
			{
				sgu8stateR = IDLE_R;
			}
			break;
		case RPM_ID2:
			if('M' == u8data)
			{
				sgu8stateR = RPM_ID3;
			}
			else
			{
				sgu8stateR = IDLE_R;
			}
			break;
		case RPM_ID3:
			if(':' == u8data)
			{
				sgu8stateR = RPM_ID4;
			}
			else
			{
				sgu8stateR = IDLE_R;
			}
			break;
		case RPM_ID4:
			if(u8data >= '0' && u8data <= '9')
			{
				sgu8stateR = RPM_PAR1;
				sgu8RpmDigits[0] = u8data - 0x30;
			}
			else
			{
				sgu8stateR = IDLE_R;
			}
			break;
		case RPM_PAR1:
			if(u8data >= '0' && u8data <= '9')
			{
				sgu8stateR = RPM_PAR2;
				sgu8RpmDigits[1] = u8data - 0x30;
			}
			else
			{
				sgu8stateR = IDLE_R;
			}
			break;
		case RPM_PAR2:
			if(u8data >= '0' && u8data <= '9')
			{
				sgu8stateR = RPM_PAR3;
				sgu8RpmDigits[2] = u8data - 0x30;
			}
			else
			{
				sgu8stateR = IDLE_R;
			}
			break;
		case RPM_PAR3:
			if('.' == u8data)
			{
				sgu8stateR = RPM_PAR4;
			}
			else
			{
				sgu8stateR = IDLE_R;
			}
			break;
		case RPM_PAR4:
			if(u8data >= '0' && u8data <= '9')
			{
				sgu16RpmSpeed = sgu8RpmDigits[0]*1000 + sgu8RpmDigits[1]*100 + sgu8RpmDigits[2]*10 + (u8data - 0x30);
			}
			sgu8stateR = IDLE_R;
			break;
		
		default:
			break;
	}
}


