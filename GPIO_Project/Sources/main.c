#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */





void main(void) {
	PTADD = 0x08;
	//Ponemos el bit 3 en 1, por default esta en 0
  EnableInterrupts;
  /* include your code here */

  for(;;) {
	  PTAD_PTAD3 = -PTAD_PTAD3;
    __RESET_WATCHDOG();	/* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
