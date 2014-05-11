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
			
			MOV #0FFH,PTBDD
			;Salidas a PTBD
			MOV #03H, PTADD
			;Configurar pin 0 como RS y pin 1 como E de PTA0 como salida. RW siempre es 0
					
mainLoop:
            ; Insert your code here
inicio:
			;Poner RS en 0, modo instruccion
			LDA #00H
			STA PTAD			
			;Comando lcd clear
			LDA #01H
			STA PTBD
			JSR comando_inst
			;Configurar primer linea
			LDA #0CH
			STA PTBD
			JSR comando_inst
			;Configurar cursor, set
			LDA #38H
			STA PTBD
			JSR comando_inst
			;Poner RS en 1, modo dato
			LDA #01H
			STA PTAD
			JSR mensaje
aqui:     	BRA aqui
			
mensaje:
			LDA #46H
			STA PTBD
			JSR comando_dato
			LDA #45H
			STA PTBD
			JSR comando_dato
			LDA #52H
			STA PTBD
			JSR comando_dato
			LDA #52H
			STA PTBD
			JSR comando_dato
			LDA #41H
			STA PTBD
			JSR comando_dato
			LDA #53H
			STA PTBD
			JSR comando_dato
			LDA #20H
			STA PTBD
			JSR comando_dato
			LDA #59H
			STA PTBD
			JSR comando_dato
			LDA #41H
			STA PTBD
			JSR comando_dato
			LDA #20H
			STA PTBD
			JSR comando_dato
			LDA #45H
			STA PTBD
			JSR comando_dato
			LDA #53H
			STA PTBD
			JSR comando_dato
			LDA #54H
			STA PTBD
			JSR comando_dato
			LDA #55H
			STA PTBD
			JSR comando_dato
			LDA #56H
			STA PTBD
			JSR comando_dato
			LDA #4FH
			STA PTBD
			JSR comando_dato
			RTS
			
comando_inst:
			;Poner enable en 1
			LDA #02H
			STA PTAD       
			JSR delay
			JSR delay
			;Poner enable en 0
			LDA #00H
			STA PTAD
			JSR delay
			RTS
			
comando_dato:
			;Poner enable en 1
			LDA #03H
			STA PTAD       
			JSR delay
			JSR delay
			;Poner enable en 0
			LDA #01H
			STA PTAD
			JSR delay
			RTS
			
           
delay:		
			MOV #0FFH,61H
loop1:		
			MOV #0FFH,60H
loop0:		
			DBNZ 60H,loop0
			feed_watchdog
			DBNZ 61H,loop1
			RTS
