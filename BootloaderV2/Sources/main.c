#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "FlashWritter.h"
#include "SerialManager.h"

unsigned int direccion = 0xE200;
unsigned char DATO_FLASH = 0x65;
unsigned char ERROR_FLASH = 0;  
unsigned char erased = 1;

void main(void) {

	//EnableInterrupts;
	//SerialManager_Init();
	FCDIV = 0x13;
	if(FlashErase(direccion))
	{
		ERROR_FLASH = FlashWrite(direccion,&DATO_FLASH);
	}
	for(;;) {
		__RESET_WATCHDOG();	/* feeds the dog */
		
	} /* loop forever */
	/* please make sure that you never leave main */
}
