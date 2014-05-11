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

#define PortB 1
#define Enable 4
#define RS 5
#define u8set 1
#define u8clear 0
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

unsigned int u16remainder;
unsigned char u8LCD_Output = 0x3F;
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
char id_20ms = 0;
char id_5ms = 0;
char id_100us = 0;
int u16speed = 0;
char u8speed = 0;

void delay_5ms(void);
void delay_100us(void);
void delay_20ms(void);
void Send_delay(void);
void Send_cmd(char comand);
void Send_delay_data(void);
void Send_data(char data);
void Send_cmd_small(char comand);



void LcdManager_Init(void)
{
	MTIM_Init();
	MTIM_SetTimerModulo(50);
	MTIM_SelectPrescaler(2);
	MTIM_InterruptOn();
	
	//Delay inicial
	//Inicializacion en 4 bits

	GPIO_ConfigPortDirection_Mask(PortB, u8LCD_Output);

	delay_20ms();
	
	GPIO_WritePort(PortB, u8Initial_Config_A);
	Send_delay();
	
	GPIO_WritePort(PortB, u8Initial_Config_A);
	Send_delay();
	
	GPIO_WritePort(PortB, u8Initial_Config_A);
	Send_delay();
	
	GPIO_WritePort(PortB, u8Initial_Config_A);
	Send_delay();
	
	GPIO_WritePort(PortB, u8Initial_Config_B);
	Send_delay();
	
	Send_cmd(0x28); //Set 4 bit, 2 lines
	Send_cmd(0x0C); //Display on cursor off, first line
	Send_cmd(0x06); //Automatic Increment
	Send_cmd(0x01); //Clear display
	
}


unsigned int LcdManager_RemainderTask(void)
{
	return 0;
}


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
}

/*************************************************************************************************/
/*********************				    Private Functions					**********************/
/*************************************************************************************************/
void delay_5ms(void)
{
	MTIM_SetExtraFlags(49);
	MTIM_Start();
	do
	{
		__RESET_WATCHDOG();
	}while(MTIM_IsRunning());
}
void delay_100us(void)
{
	MTIM_SetExtraFlags(0);
	MTIM_Start();
	do
		{
			__RESET_WATCHDOG();
		}while(MTIM_IsRunning());
}
void delay_20ms(void)
{
	MTIM_SetExtraFlags(199);
	MTIM_Start();
	do
		{
			__RESET_WATCHDOG();
		}while(MTIM_IsRunning());
}
void Send_delay(void)
{
	GPIO_WritePin(PortB, Enable, u8set);
	delay_100us();
	GPIO_WritePin(PortB, Enable, u8clear);
	delay_5ms();
}

void Send_cmd(char comand)
{
	char nibble_1 = (comand >> 4) & 0x0F;
	char nibble_2 = comand & 0x0F;
	GPIO_WritePort(PortB, nibble_1);
	Send_delay();
	GPIO_WritePort(PortB, nibble_2);
	Send_delay();
}


void Send_delay_data(void)
{
	GPIO_WritePin(PortB, Enable, u8set);
	delay_100us();
	GPIO_WritePin(PortB, Enable, u8clear);
	delay_100us();
}

void Send_data(char data)
{
	char nibble_1 = (data >> 4) & 0x0F;
	char nibble_2 = data & 0x0F;
	GPIO_WritePort(PortB, nibble_1);
	GPIO_WritePin(PortB, RS, u8set);
	Send_delay_data();
	GPIO_WritePort(PortB, nibble_2);
	GPIO_WritePin(PortB, RS, u8set);
	Send_delay_data();
}
