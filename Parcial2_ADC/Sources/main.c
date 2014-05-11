#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "ADC.h"
#include "GPIO.h"

char *conversion_value;
char conversion = 0;
char u8input = 0;
char u8output = 1;
char u8PortB = 1;
char u8PortA = 0;

void init_All()
{
	conversion_value = &conversion;
	
	GPIO_ConfigPortDirection(u8PortB, u8output);

}


void main(void) {
	int shit = 0x0F11F;
	char shit_2 = 0;
	shit = shit >> 9;
	shit_2 = (char)shit;
	
  EnableInterrupts;
  init_All();
	while(3)
	{
		ADC_Init();
	}
  for(;;) {
	  ADC_StartConversion();
	  while(!(ADC_Read_8(conversion_value))){
		  
	  }
	  GPIO_WritePort(u8PortB, conversion);
    __RESET_WATCHDOG();	/* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
