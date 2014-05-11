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
			
			
			MOV #0F0H,PTBDD ;Configurar puerto
			CLR 63H ;63H bit0 -> secuencia que ejecutara
			
mainLoop:
            ; Insert your code here         
empezar:    
			LDX #04 ;inicializar contador
			LDA #10H ;Inicializar barrido
			BRCLR 3,63H,secuencia1
;secuencia2----------------------------------
secuencia2: STA PTBD
			JSR delay
			ASLA
			DBNZX secuencia2
			LDX #03
			LDA #40H
shifth_der:	STA PTBD
			JSR delay
			LSRA
			DBNZX shifth_der
			LDX #03	
			LDA #20H
			BRA secuencia2
			
;secuencia1----------------------------------		
secuencia1: STA PTBD
			JSR delay
			ASLA
			DBNZX secuencia1
			LDX #04
			LDA #10H
			BRA secuencia1

            feed_watchdog
            BRA    mainLoop

;delay---------------------------------------
delay:		
			
			MOV #04H,62H
loop2:		MOV #0FFH,61H
loop1:		MOV #0FFH,60H
loop0:		NOP
			DBNZ 60H,loop0
			feed_watchdog
			DBNZ 61H,loop1
			DBNZ 62H,loop2
			
			BRCLR 3,PTBD,sigue
			AIS #-01H
			BRCLR 3,63H,prende
			CLR 63H
			BRA empezar
prende:		MOV #08H,63H
			BRA empezar	
			
sigue:		RTS


