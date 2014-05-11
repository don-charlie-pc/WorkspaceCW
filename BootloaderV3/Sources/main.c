#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "bootloader.h"
#include "SerialManager.h"

/* Flash access function must be allocated in RAM */
#pragma MESSAGE DISABLE C1805

word address = 0xE400;
byte data_byte = 0x65;
byte u8error = 0;
byte u8new_data = 0;
byte u8complete_data = 0;
byte u8mask_write = 0x13;
byte finish_write = 0;


void main(void) {

	EnableInterrupts;
	SerialManager_Init();
	Bootloader_FlashErase(address);
	/* include your code here */
	for(;;) {
		SerialManager_GetValue(&u8new_data, &data_byte, &u8complete_data, &finish_write);
		__RESET_WATCHDOG();	/* feeds the dog */
		if((u8new_data == 1) && (u8complete_data==1))
		{
			FCDIV = u8mask_write;
			if(TRUE){
				u8error=Bootloader_FlashWrite(address,&data_byte,1);
				address++;
			}
		}
		if(finish_write)
		{
			__asm{
				JMP address;
			}
		}
	} /* loop forever */
	/* please make sure that you never leave main */
}

