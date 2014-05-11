/*! **********************************************************************************************
* 2014  ITESM Campus Guadalajara. Laboratorio de Microcontroladores 
*  
*
* @file:      FlashWritter.c
* @author(s):  
* Abner Perez Haro A01224776
* Juan Carlos Perez Castellanos A01225629
* 
* @brief (Theory of Operation)
*
**************************************************************************************************/


/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/

/* Flash access function must be allocated in RAM */
#pragma MESSAGE DISABLE C1805
#define Page_Erase   ProgramASM[21] = 0x40; temp = ((unsigned char(*)(unsigned int))(ProgramASM))
#define Byte_Program ProgramASM[21] = 0x20; temp = ((unsigned char(*)(unsigned int, unsigned char))(ProgramASM))
/*ProgramASM[21] is the byte assigned to the FCMD register.
	FCMD - Blank check 0x05
	 	 - Byte program 0x20
	 	 - Burst program 0x25
	 	 - Page erase 0x40
	 	 - Mass erase 0x41
*/
//temp assigns the result of the function to the ProgramASM

 
/* Array of opcode instructions of the Erase/Program function in the HCS08 family */
volatile unsigned char ProgramASM[64] = 
{  
	0x87,0xC6,0x18,0x25,0xA5,0x10,0x27,0x08,0xC6,0x18,0x25,0xAA,0x10,0xC7,0x18,0x25,
	0x9E,0xE6,0x01,0xF7,0xA6,0x20,0xC7,0x18,0x26,0x45,0x18,0x25,0xF6,0xAA,0x80,0xF7,
	0x9D,0x9D,0x9D,0x9D,0x45,0x18,0x25,0xF6,0xF7,0xF6,0xA5,0x30,0x27,0x04,0xA6,0xFF,
	0x20,0x07,0xA6,0xFF,0xC7,0x18,0x00,0xC6,0x18,0x25,0xA5,0x40,0x27,0xF4,0x8A,0x81
};
/*  The opcode above represents this set of instructions  
    if ( FSTAT & 0x10 )
		{ 
		  // Check to see if FACCERR is set
			FSTAT = FSTAT | 0x10;    // write a 1 to FACCERR to clear
    }
    (*((volatile unsigned char *)(Address))) = data;  //write to somewhere in flash
    FSTAT = FSTAT | 0x80;      //Put FCBEF at 1.
    _asm NOP;                  //Wait 4 cycles
    _asm NOP;
    _asm NOP;
    _asm NOP;
    if ( FSTAT & 0x30 )
		{  
		  //check to see if FACCERR or FVIOL are set
			return 0xFF;             //if so, error.
    }
    while (( FSTAT & 0x40 )== 0 )
		{ 
			// Kick the dog 
			_asm LDA 0xFF 
			_asm STA 0x1800
    }
   */

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

/*************************************************************************************************/
/*********************                  Static Constants                    **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************                  Global Constants                    **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************				   Exported Functions					**********************/
/*************************************************************************************************/

unsigned char FlashErase(unsigned int page_address)
{
	unsigned char temp;
	unsigned char erased = 1;
	// To not access the flash while erasing
	DisableInterrupts;
	//When assigning to temp, starts the ProgramASM with configuration to erase page.
	temp = Page_Erase(page_address); 
	if (0xFF == temp)
	{
		// Erasing error 
		erased = 0;
	} 
	EnableInterrupts;
	// Erasing successful
	return(erased);
}

unsigned char FlashWrite(unsigned int address, unsigned char *source, unsigned char size)
{
	unsigned char i, temp;
	unsigned char written = 1;
	// To not access the flash while erasing
	DisableInterrupts;
	for(i = 0; i<size; i++)
	{
		//When assigning to temp, starts the ProgramASM with configuration to write byte
		temp = Byte_Program(address+i, (unsigned char)*(source+i));
		if(0xFF == temp)
		{
			//Writing error
			written = 0;
			break;
		}
	} 
	EnableInterrupts;
	//Writing successful
	return written;
}
/*************************************************************************************************/
/*********************				    Private Functions					**********************/
/*************************************************************************************************/

