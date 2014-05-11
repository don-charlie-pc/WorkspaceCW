#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

void main(void) {
	unsigned char state = 0;
	unsigned char cuenta = 0;
  EnableInterrupts;
  /* include your code here */ 
  for(;;) {
	state = 0;
	while(cuenta < 250)
	{
		cuenta++;
	}
	state = 1;
	cuenta = 0;
	while(cuenta < 250)
	{
		cuenta++;
	}
    __RESET_WATCHDOG();	/* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
