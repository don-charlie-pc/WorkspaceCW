/*! **********************************************************************************************
* 2014  ITESM Campus Guadalajara. Laboratorio de Microcontroladores 
*  
*
* @file:      bootloader.c
* @author(s):  
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
#define erase_page program_ASM[21] = 0x40; temp = (( byte (*)( word ))( program_ASM ))
#define write_byte_program program_ASM[21] = 0x20; temp = (( byte (*)( word, byte ))( program_ASM ))
/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/
#ifndef __HIWARE__

typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned long dword;
#else

typedef unsigned char byte;
typedef unsigned int word;
typedef unsigned long dword;
#endif 

typedef unsigned char bool;
#ifndef FALSE
#define FALSE 0                    
#endif
#ifndef TRUE
#define TRUE  !FALSE               
#endif
/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************                  Static Variables                    **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Variables					**********************/
/*************************************************************************************************/
volatile unsigned char program_ASM[64]  = 
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
/*********************                  Static Constants                    **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************                  Global Constants                    **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************				   Exported Functions					**********************/
/*************************************************************************************************/
bool Bootloader_FlashErase( word page_addr )
{
	byte temp;
	bool erased = TRUE;

	/* In this routine is not allowed access to flash */
	DisableInterrupts;

	temp = erase_page( page_addr );

	if ( temp == 0xFF )
	{
		/* Signal Error */
		erased = FALSE;
	}

	EnableInterrupts;

	/* Signal Ok */
	return erased;
}

byte Bootloader_FlashWrite( word address_byte, byte *data_source, byte len )
{
	byte i, temp;
	bool written = TRUE;

	/* In this routine is not allowed access to flash */
	DisableInterrupts;

	for ( i = 0; i < len; i ++ )
	{
		temp = write_byte_program( address_byte + i, ( byte ) *( data_source + i ));

		/* Execute Program routine */
		if ( temp == 0xFF )
		{
			/* Signal Error */
			written = FALSE;
			break;
		}
	}

	EnableInterrupts;

	/* Signal Ok */
	return written ;
}

/*************************************************************************************************/
/*********************				    Private Functions					**********************/
/*************************************************************************************************/

