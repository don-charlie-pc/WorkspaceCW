#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

static unsigned int DIRECCION = 0xE382;
void main(void) {
	unsigned int DIRECCION = 0xE200;
	unsigned char DATO_FLASH = 0x65;
	unsigned char ERROR_FLASH = 0;  
	unsigned char erased = 0;
	FCDIV = 0x13;
	//EnableInterrupts;
	erased = FlashErase(DIRECCION);
	if(erased)
	{
		ERROR_FLASH = FlashWrite(DIRECCION,&DATO_FLASH,1);
	}
	for(;;) {
		__RESET_WATCHDOG();	/* feeds the dog */

	} /* loop forever */
	/* please make sure that you never leave main */
}
