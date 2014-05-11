#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */


void MCU_init(void)
{
	/* ### MC9S08QG8_16 "Cpu" init code ... */
	/* PE initialization code after reset */
	/* System clock initialization */
	/* SOPT1: COPE=0,COPT=1,STOPE=0,BKGDPE=0,RSTPE=0 */
	SOPT1 = 0x50;
	/* SPMSC1: LVDF=0,LVDACK=0,LVDIE=0,LVDRE=1,LVDSE=1,LVDE=1,BGBE=0 */
	ICSC1 = 0x04; /* Initialization of the ICS control register 1 */
	/* ICSC2: BDIV=1,RANGE=0,HGO=0,LP=0,EREFS=0,ERCLKEN=0,EREFSTEN=0 */
	ICSC2 = 0x40; /* Initialization of the ICS control register 2 */
	/* Common initialization of the write once registers */
	/* SOPT2: COPCLKS=0,IICPS=0,ACIC=0 */
	SOPT2 = 0x00;
	/* FCDIV: DIVLD=0,PRDIV8=0,DIV5=0,DIV4=1,DIV3=0,DIV2=0,DIV1=1,DIV0=1 */
	FCDIV = 0x13;
}

//Array of opcode instructions of the Erase/Program function
//Element 0x14 of the array is: (command 0x20 to program a byte, 0x40 to erase a page)
unsigned char FLASH_CMD[] = {
		0x87,0xC6,0x18,0x25,0xA5,0x10,0x27,0x08,0xC6,0x18,0x25,0xAA,0x10,0xC7,0x18,0x25,
		0x9E,0xE6,0x01,0xF7,0xA6,0x20,0xC7,0x18,0x26,0x45,0x18,0x25,0xF6,0xAA,0x80,0xF7,
		0x9D,0x9D,0x9D,0x9D,0x45,0x18,0x25,0xF6,0xF7,0xF6,0xA5,0x30,0x27,0x04,0xA6,0xFF,
		0x20,0x07,0xC6,0x18,0x25,0xA5,0x40,0x27,0xF9,0x8A,0x81};
/* The opcode above represents this set of instructions
if (FSTAT&0x10){ //Check to see if FACCERR is set
FSTAT = FSTAT | 0x10; //write a 1 to FACCERR to clear
}
(*((volatile unsigned char *)(Address))) = data; //write to somewhere in flash
FCMD = 0x20; //set command type.
FSTAT = FSTAT | 0x80; //Put FCBEF at 1.
_asm NOP; //Wait 4 cycles
_asm NOP;
_asm NOP;
_asm NOP;
if (FSTAT&0x30){ //check to see if FACCERR or FVIOL are set
return 0xFF; //if so, error.
}
while ((FSTAT&0x40)==0){ //else wait for command to complete
;
}*/


//6. Erase a 512 byte page starting where the first value was written
void Erase(int Address){
	unsigned char dummy;
	FLASH_CMD[21] = 0x40; //Erase command is written into the array
	asm jsr FLASH_CMD; //jumps to where the Erase Routine is located at
	asm sta dummy;
	if ( dummy == 0xFF){ asm NOP;} //An error occurred during the Erasing of the FLASH
}
//5. Program one byte in the Flash memory
void Program(int Address, unsigned char data){
	unsigned char dummy;
	asm jsr FLASH_CMD; //jumps to where the Program Routine is located at
	asm sta dummy;
	if (dummy == 0xFF){
		asm NOP; } //An error occurred during the Programming of the FLASH
}

void main(void) {
	unsigned char counter;
	EnableInterrupts;
	MCU_init();
	
	/* include your code here */
	//3. Disable interrupts to permit execution of the code
	DisableInterrupts;

	//4. Cycle that writes a byte form value 0 to 127
	Erase(0x3200);
	for(counter=0;counter<=127;counter++)
	{
		Program(0x3200 + counter, counter);
	}
	for(;;) {
		__RESET_WATCHDOG();	/* feeds the dog */
	} /* loop forever */
	/* please make sure that you never leave main */
}
