/*! **********************************************************************************************
* 2014  ITESM Campus Guadalajara. Laboratorio de Microcontroladores 
*  
*
* @file:      LcdManager.c
* @author(s):  
*
* @brief (Theory of Operation)
*
**************************************************************************************************/


/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
#include "GPIO.h"
#include "MTIM.h"
#include "derivative.h"
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
//Configure to change the default pins used for LCD
#define LCD_CONTROL_BITS_MASK 0x0C
#define LCD_DATA_BITS_MASK 0xF0
#define D4_PIN 4
#define D5_PIN 5
#define D6_PIN 6
#define D7_PIN 7

//Pins
#define ENABLE_PIN 2
#define RS_PIN 3

//States
#define INIT 0
#define IDLE 1
#define SET_POSITION 2
#define WRITE_CHAR 3

//Commands
#define EIGHT_BIT_CONFIG 0x03
#define FOUR_BIT_CONFIG 0x02

/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/
char LcdManager_IsBusy(void);
void LcdManager_SetRequestedPosition(void);
void LcdManager_WriteDataPort(char u8data);
void LcdManager_delay_5ms(void);
void LcdManager_delay_100us(void);
void LcdManager_delay_20ms(void);
void LcdManager_Send_delay(void);
void LcdManager_Send_cmd(char comand);
void LcdManager_Send_delay_data(void);
void LcdManager_Send_data(char data);
/*************************************************************************************************/
/*********************                  Static Variables                    **********************/
/*************************************************************************************************/
//Backed up parameters to write message
static char sgu8linea;
static char sgu8offset;
static char * sgpu8String; 
static char sgu8counter;

static char sgu8currentState = INIT;
/*************************************************************************************************/
/*********************					Global Variables					**********************/
/*************************************************************************************************/
unsigned int u16remainder;
unsigned char u8Initial_Config_A = 0x03;
unsigned char u8Initial_Config_B = 0x02;
unsigned int u16adcVoltage = 0;
char u8Voltage_cen = 0;
char u8Voltage_dec = 0;
char u8Voltage_uni = 0;
char u8RPM_cen = 0;
char u8RPM_dec = 0;
char u8RPM_uni = 0;
char u8adcVoltage = 0;
int u16speed = 0;
char u8speed = 0;
char sgu8LcdBusyFlag = 0;
/*************************************************************************************************/
/*********************                  Static Constants                    **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************                  Global Constants                    **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************				   Exported Functions					**********************/
/*************************************************************************************************/

void LcdManager_Init(void)
{
	char u8count;
	
	//Set up MTIM
	MTIM_Init();
	MTIM_SetTimerModulo(100);
	(void)MTIM_SelectPrescaler(2);
	
	//Configure LCD data pins
	GPIO_ConfigPortOutputsByMask(PORTB,LCD_DATA_BITS_MASK);
	GPIO_ConfigPortOutputsByMask(PORTA,LCD_CONTROL_BITS_MASK);
	
	//Initial delay
	LcdManager_delay_20ms();
	
	//4 bits configuration
	//TODO
	u8count = 4;
	do
	{
		LcdManager_WriteDataPort(EIGHT_BIT_CONFIG);
		LcdManager_Send_delay();
	}while(--u8count);	
	LcdManager_WriteDataPort(FOUR_BIT_CONFIG);
	LcdManager_Send_delay();
	
	//Set 4 bit, 2 lines
	LcdManager_Send_cmd(0x28); 
	//Display on cursor off, first line
	LcdManager_Send_cmd(0x0C); 
	//Automatic Increment
	LcdManager_Send_cmd(0x06); 
	//Clear display
	LcdManager_Send_cmd(0x01); 
	
	//Change to IDLE state
	sgu8currentState = IDLE;
}

void LcdManager_UpdateTask(void)
{
	if(sgu8currentState != INIT)
	{	
		switch(sgu8currentState)
		{
			case IDLE:
				if(sgu8LcdBusyFlag == 1)
				{
					sgu8currentState = SET_POSITION;
				}
				break;
			case SET_POSITION:
				sgu8currentState = WRITE_CHAR;
				LcdManager_SetRequestedPosition();
				break;
			case WRITE_CHAR:
				LcdManager_Send_data(*sgpu8String);
				sgpu8String++;
				sgu8counter--;
				if(sgu8counter==0)
				{
					sgu8currentState = IDLE;
					sgu8LcdBusyFlag = 0;
				}
				break;
			default:
				break;
		}
	}
}

void LcdManager_WriteString(char u8linea, char u8offset, char * pu8String, char u8length)
{
	sgu8LcdBusyFlag = 1;
	
	sgu8linea = u8linea;
	sgu8offset = u8offset;
	sgpu8String = pu8String; 
	sgu8counter = u8length;
}

/*
unsigned int LcdManager_Write(char u8speedCurrent, char u8adcValue, char u8dir, char u16motorDelay)
{
	//Convertir u8adcValue a char
	Send_cmd(0x02); //Enviar a primer linea
	u16speed = ((u8speedCurrent+1)*11)+1;

	u8RPM_cen = (u16speed/100);
	u8speed = (u16speed%100);
	u8RPM_dec = u8speed/10;
	u8RPM_uni = u8speed%10;
	
	Send_data(u8RPM_cen+(0x30));
	Send_data(u8RPM_dec+(0x30));
	Send_data(u8RPM_uni+(0x30));

	Send_data(' ');
	Send_data('R');
	Send_data('P');
	Send_data('M');
	Send_data('(');

	//Dejar voltaje en rango 0-330
	u16adcVoltage = ((129*u8adcValue)/100);
	u8Voltage_cen = (u16adcVoltage/100);
	u8adcVoltage = (u16adcVoltage%100);
	u8Voltage_dec = u8adcVoltage/10;
	u8Voltage_uni = u8adcVoltage%10;
	
	Send_data(u8Voltage_cen+0x30);
	Send_data('.');
	Send_data(u8Voltage_dec+0x30);
	Send_data(u8Voltage_uni+0x30);
	Send_data('V');
	Send_data(')');
	
	//Salto Linea 2
	Send_cmd(0xC0);
	if(u8dir == 0)
	{
		Send_data('I');
		Send_data('z');
		Send_data('q');
		
	}
	else
	{
		Send_data('D');
		Send_data('e');
		Send_data('r');		
	}
	return 0;
}*/

/*************************************************************************************************/
/*********************				    Private Functions					**********************/
/*************************************************************************************************/

char LcdManager_IsBusy(void)
{
	return sgu8LcdBusyFlag;
}

void LcdManager_SetRequestedPosition(void)
{
	//TODO
	//LcdManager_Send_cmd(0x01); //Clear display
	LcdManager_Send_cmd(0x02);
	//LcdManager_Send_cmd(0x40+sgu8offset);
}

void LcdManager_delay_5ms(void)
{
	MTIM_SetExtraFlags(49);
	MTIM_Start();
	do
	{
		__RESET_WATCHDOG();
	}while(MTIM_IsRunning());
}

void LcdManager_delay_100us(void)
{
	MTIM_SetExtraFlags(0);//TODO
	MTIM_Start();
	do
	{
		__RESET_WATCHDOG();
	}while(MTIM_IsRunning());
}

void LcdManager_delay_20ms(void)
{
	MTIM_SetExtraFlags(199);
	MTIM_Start();
	do
	{
		__RESET_WATCHDOG();
	}while(MTIM_IsRunning());
}

void LcdManager_Send_delay(void)
{
	GPIO_WriteBitToPin(PORTA, ENABLE_PIN, ON);
	LcdManager_delay_100us();
	GPIO_WriteBitToPin(PORTA, ENABLE_PIN, OFF);
	LcdManager_delay_5ms();
}

void LcdManager_Send_cmd(char comand)
{
	char u8nibble_1 = (comand >> 4) & 0x0F;
	char u8nibble_2 = comand & 0x0F;
	GPIO_WriteBitToPin(PORTA, RS_PIN, OFF);
	LcdManager_WriteDataPort(u8nibble_1);
	LcdManager_Send_delay();
	LcdManager_WriteDataPort(u8nibble_2);
	LcdManager_Send_delay();
}

void LcdManager_Send_delay_data(void)
{
	GPIO_WriteBitToPin(PORTA, ENABLE_PIN, ON);
	LcdManager_delay_100us();
	GPIO_WriteBitToPin(PORTA, ENABLE_PIN, OFF);
	LcdManager_delay_100us();
}

void LcdManager_Send_data(char data)
{
	char u8nibble_1 = (data >> 4) & 0x0F;
	char u8nibble_2 = data & 0x0F;
	GPIO_WriteBitToPin(PORTA, RS_PIN, ON);
	LcdManager_WriteDataPort(u8nibble_1);
	LcdManager_Send_delay_data();
	LcdManager_WriteDataPort(u8nibble_2);
	LcdManager_Send_delay_data();
	GPIO_WriteBitToPin(PORTA, RS_PIN, OFF);
}

void LcdManager_WriteDataPort(char u8data)
{
	
	GPIO_WriteBitToPin(PORTB,D4_PIN,(u8data&0x01));
	GPIO_WriteBitToPin(PORTB,D5_PIN,(u8data>>1)&0x01);
	GPIO_WriteBitToPin(PORTB,D6_PIN,(u8data>>2)&0x01);
	GPIO_WriteBitToPin(PORTB,D7_PIN,(u8data>>3)&0x01);
}
