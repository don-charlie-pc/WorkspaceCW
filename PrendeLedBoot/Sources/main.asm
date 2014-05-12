;*******************************************************************
;* This stationery serves as the framework for a user application. *
;* For a more comprehensive program that demonstrates the more     *
;* advanced functionality of this processor, please see the        *
;* demonstration applications, located in the examples             *
;* subdirectory of the "Freescale CodeWarrior for HC08" program    *
;* directory.                                                      *
;*******************************************************************

; Include derivative-specific definitions
            INCLUDE 'derivative.inc'
            

; export symbols
            XDEF _Startup, main
            ; we export both '_Startup' and 'main' as symbols. Either can
            ; be referenced in the linker .prm file or from C/C++ later on
            
            
            
            XREF __SEG_END_SSTACK   ; symbol defined by the linker for the end of the stack


; variable/data section
MY_ZEROPAGE: SECTION  SHORT         ; Insert here your data definition

; code section
MyCode:     SECTION
main:
_Startup:
            LDHX   #__SEG_END_SSTACK ; initialize the stack pointer
            TXS
			;CLI			; enable interrupts

mainLoop: 
            MOV #0F0H,PTBDD ;Configurar puerto


;codigo nuevo

	
downled:
			MOV #01H,62H
loop12:		MOV #0FFH,61H
loop11:		MOV #0FFH,60H
loop10:		NOP
			DBNZ 60H,loop10
			feed_watchdog
			DBNZ 61H,loop11
			DBNZ 62H,loop12
			
			LDA #0F0H
			STA PTBD
			
			MOV #01H,62H			
loop16:		MOV #0FFH,61H
loop15:		MOV #0FFH,60H
loop14:		NOP
			DBNZ 60H,loop14
			feed_watchdog
			DBNZ 61H,loop15
			DBNZ 62H,loop16


			LDA #00H
			STA PTBD
			BRA downled
;*********************************
			
			MOV #04H,62H
loop2:		MOV #0FFH,61H
loop1:		MOV #0FFH,60H
loop0:		NOP
			DBNZ 60H,loop0
			feed_watchdog
			DBNZ 61H,loop1
			DBNZ 62H,loop2
			;Prender LEDs
			LDA #0F0H
			STA PTBD
			
			MOV #04H,62H			
loop6:		MOV #0FFH,61H
loop5:		MOV #0FFH,60H
loop4:		NOP
			DBNZ 60H,loop4
			feed_watchdog
			DBNZ 61H,loop5
			DBNZ 62H,loop6
			
			;Apagar LEDs
			LDA #00H
			STA PTBD
			BRA mainLoop
