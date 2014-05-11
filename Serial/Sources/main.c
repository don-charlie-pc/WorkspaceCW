#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include  "GPIO.h"

unsigned char voltage[] = {" .  V"};
//Programa que envia valor de ADC por serial
interrupt 19 void AdcISR(void)
{
	unsigned char count = 0;
	unsigned int Adc;
	ADCSC1 = 0x20;
	Adc = (unsigned int)ADCRL;
	
	
	//Al leer, se apaga la bandera
	Adc = (Adc*13);
	Adc /= 10;
	voltage[3] = (Adc%10) + 0x30;
	Adc /= 10;
	voltage[2] = (Adc%10) + 0x30;
	voltage[0] = (Adc/10) + 0x30;
	
	do
	{
		do
		{
			__RESET_WATCHDOG();
			
		}
		while(!SCIS1_TDRE);
		SCID = voltage[count];
		count++;
		/*while(!GPIO_ReadPin(PORTA,1))
		{
			__RESET_WATCHDOG();
		}*/
	}
	while(count<5);
	ADCSC1 = 0x20;
}


void main(void) {
    unsigned char count = 0;
    GPIO_ConfigPinDirection(PORTA,1,INPUT);
    while(!GPIO_ReadPin(PORTA,1))
    		{
    			__RESET_WATCHDOG();
    		}
    
    
	
	EnableInterrupts;
  /* include your code here */
  
  
  
  //Para el serial
    SCIBD = 26;
    SCIC2 = 0x0C;
    
  //Para el ADC
  //APCTL1 = 0x01; //Este si
  
  //ADCCFG = 0xA0;
  //ADCSC1 = 0x40; //Este si


    	count = 0;
    	voltage[3] = 0x31;
    	voltage[2] = 0x31;
    	voltage[0] = 0x31;
    	
    	do
    	{
    		do
    		{
    			__RESET_WATCHDOG();
    			
    		}
    		while(!SCIS1_TC);
    		SCID = voltage[count];
    		count++;

    	}
    	while(count<5);
    
    
    
    
  for(;;) {
    __RESET_WATCHDOG();	/* feeds the dog */

  } /* loop forever */
  /* please make sure that you never leave main */
}
