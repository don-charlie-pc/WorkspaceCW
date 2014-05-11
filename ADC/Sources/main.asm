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
			CLI			; enable interrupts
						;Configurar puerto
			MOV #0FFH,PTBDD
			;Salidas a PTBD
			MOV #00H, PTADD
			;Configurar puerto PTA0 como input de ADC
			;ADPC0 es el pin control
			MOV #00H, ADCCFG
			MOV #00H, ADCSC2
		    MOV #00H, ADCSC1
		    MOV #03H, APCTL1
			;CONFIGURACIONES Y ACTIVAR ADC
mainLoop:
            ;ADCRL TIENE EL VALOR DE LA CONVERSION DE 8 BITS
			BCLR 7, ADCSC1
			;CLR COCO, ACTIVA CONVERSION
			
			
			;DELAY
			MOV #04H,62H
loop2:		MOV #0FFH,61H
loop1:		MOV #0FFH,60H
loop0:		NOP
			DBNZ 60H,loop0
			feed_watchdog
			DBNZ 61H,loop1
			DBNZ 62H,loop2
			
			
			LDA ADCRL 
			;Obtener valores obtenidos por el ADC
			STA PTBD
			;Pasar valores del ADC a PTBD
            BRA    mainLoop


