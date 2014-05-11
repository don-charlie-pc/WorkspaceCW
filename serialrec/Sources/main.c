#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

unsigned char letra;
unsigned char val;

//Programa que recibe char por serial
interrupt 15 void SCIreceive(void)
{
	//Apagar la bandera
	(void) SCIS1;
	//Guardar letra
	letra = SCID;
	val = letra;
}


void main(void) {


	
	EnableInterrupts;
  /* include your code here */
  
  
  
  //Para el baud rate
    SCIBD = 26;
    //Habilitar receiver interrupt
    SCIC2 = 0x2C;
    //
    
  
    //		if(SCIS1_RDRF)
    	
    	
    
  for(;;) {
    __RESET_WATCHDOG();	/* feeds the dog */

  } /* loop forever */
  /* please make sure that you never leave main */
}
