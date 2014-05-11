/*********************						Includes						**********************/
/*************************************************************************************************/
#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "LcdManager.h"
#include "GPIO.h"
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/

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


//PTB7 a PTB0 son los 8 bits del ADC
//LCD datos PTA3 PTA0
//RS PTA4
//Enable PTA5

/*************************************************************************************************/
/*********************                  Static Constants                    **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************                  Global Constants                    **********************/
/*************************************************************************************************/
static char sgpu8_3bitLcdMsg[8];
static unsigned int diametro = 8; //En 8 cm
unsigned int count_to_send = 0;
//Usando de referencia 3.3 V  255=> 3.3 V   Cada bit equivale a 13mV
unsigned int factor_conversion =  13;
unsigned int u16RpmSpeed;
unsigned int u16count_of_ms;

/*************************************************************************************************/
/*********************				   Exported Functions					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************				    Private Functions					**********************/
/*************************************************************************************************/
void main(void)
{
	EnableInterrupts;
	LcdManager_Init();
	//PTB7 a PTB0 son los 8 bits del ADC
	//LCD datos PTA3 PTA0
	//RS es PTA4
	//Enable es PTA5
	GPIO_ConfigPortDirection(PORTB, INPUT);
	GPIO_ConfigPortDirection(PORTA, OUTPUT);

	for(;;){
			LcdManager_UpdateTask();
			//Leer los 8 bits del ADC. Se guarda en su valor*100 para no perder decimales
			count_to_send = (GPIO_ReadPort(PORTB) * factor_conversion)/2;
			//count_to_send contiene el valor de vueltas por segundo. Se divide
			//entre dos debido a que la llanta tiene dos lineas en una sola vuelta.
			
			if(LcdManager_IsBusy() == 0)
			{
				//Convertir a cm por segundo, en base al diametro de la llanta
				u16count_of_ms = (count_to_send*diametro);
				//Para decenas de cm
				sgpu8_3bitLcdMsg[0] = (char)(u16count_of_ms/10000) + 0x30;
				u16RpmSpeed = (u16count_of_ms%10000);
				//Unidades de cm
				sgpu8_3bitLcdMsg[1] = u16RpmSpeed/1000 + 0x30;
				sgpu8_3bitLcdMsg[2] = '.';
				u16RpmSpeed = u16RpmSpeed%1000;
				sgpu8_3bitLcdMsg[3] = u16RpmSpeed/100 + 0x30;
				sgpu8_3bitLcdMsg[4] = 'c';
				sgpu8_3bitLcdMsg[5] = 'm';
				sgpu8_3bitLcdMsg[6] = '/';
				sgpu8_3bitLcdMsg[7] = 's';
				LcdManager_WriteString(1,0,sgpu8_3bitLcdMsg,8);
			}
		__RESET_WATCHDOG();
	}	
} 
