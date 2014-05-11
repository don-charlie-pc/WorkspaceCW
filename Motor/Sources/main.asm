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
			;Guardar secuencias del motor a pasos
			MOV #80H, 64H	;1000
			MOV #0C0H, 65H	;1100
			MOV #40H, 66H	;0100
			MOV #60H, 67H	;0110
			MOV #20H, 68H	;0010
			MOV #30H, 69H	;0011
			MOV #10H, 6AH	;0001
			MOV #90H, 6BH	;1001
			
			MOV #07H, 71H ;Contador
			MOV #63H, 70H ;Apuntador a memoria
mainLoop:
            ; Insert your code here
empezar:    
			INC 70H
			LDX 70H;se guarda en X un apuntador a memoria
			LDA ,X ;se guarda en A el valor de Mem(x)
			STA PTBD
delay:				
			MOV #0FH,62H
loop2:		MOV #0FFH,61H
loop1:		MOV #0FFH,60H
loop0:		NOP
			DBNZ 60H,loop0
			feed_watchdog
			DBNZ 61H,loop1
			DBNZ 62H,loop2
			DBNZ 71H, empezar
			MOV #07H, 71H ;Contador
			MOV #63H, 70H
			BRA empezar
