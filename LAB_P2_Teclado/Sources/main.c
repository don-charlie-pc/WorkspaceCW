#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "KeyboardManager.h"



void main(void) {
	
  KeyboardManager_Init();
  EnableInterrupts;

  for(;;) {
	  __RESET_WATCHDOG();	/* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
