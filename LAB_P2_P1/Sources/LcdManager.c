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

void LcdManager_Init(void)
{
	//Delay inicial
	//Inicializacion en 4 bits
	GPIO_ConfigPortDirection_Mask(PortB, u8LCD_Output);
	delay_10ms();
	delay_10ms();
	
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
	
	Send_cmd(0x28);
	Send_cmd(0x0C);
	Send_cmd(0x06);
	Send_cmd(0x01);
}


unsigned int LcdManager_RemainderTask(void)
{
	
	return 0;
}


unsigned int LcdManager_Write(char u8speedCurrent, char u8adcValue, char u8dir, char u16motorDelay)
{
	//Convertir u8adcValue a char
	
	Send_data((u8speedCurrent/10)+(0x30)); //Decenas de speed
	Send_data((u8speedCurrent%10)+(0x30)); //Unidades de speed
	//TODO
	//Poner "rpm"
		
	return 0;
}

/*************************************************************************************************/
/*********************				    Private Functions					**********************/
/*************************************************************************************************/
void delay_5ms()
{
	
}
void delay_100us()
{
	
}
void Send_delay()
{
	GPIO_WritePin(PortB, Enable, u8set);
	delay_100us();
	GPIO_WritePin(PortB, Enable, u8clear);
	delay_5ms();
}

void Send_cmd(char comand)
{
	char nibble_1 = (comand >> 4) && 0x0F;
	char nibble_2 = comand && 0x0F;
	GPIO_WritePort(PortB, nibble_1);
	Send_delay();
	GPIO_WritePort(PortB, nibble_2);
	Send_delay();
}
void Send_delay_data()
{
	GPIO_WritePin(PortB, Enable, u8set);
	delay_100us();
	GPIO_WritePin(PortB, Enable, u8clear);
	delay_100us();
}

void Send_data(char data)
{
	char nibble_1 = (data >> 4) && 0x0F;
	char nibble_2 = data && 0x0F;
	GPIO_WritePort(PortB, nibble_1);
	GPIO_WritePin(PortB, RS, u8set);
	Send_delay_data();
	GPIO_WritePort(PortB, nibble_2);
	GPIO_WritePin(PortB, RS, u8set);
	Send_delay_data();
}
