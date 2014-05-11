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

mainloop:

			MOV #31, 60H ;I
			MOV #31, 61H ;J
			MOV #13, 62H ;K
	
			;mul se hace x;a
ciclo_k:
			LDA 62H
			LDX 62H
			MUL
			PSHA
			PSHX
			;Genera k^4 y la guarda en el SP
ciclo_j:    
			LDA 61H
		    LDX 61H
		    MUL 
		    MUL
		    ;Genera j^3 y la guarda en el SP
ciclo_i:
			LDA 60H
		    LDX 60H
		    ;Genera i^2 y guarda en el SP
			
****************************************************************************************************
multiplicacion_16b:
			LDA 0A1H
			LDX 0A3H
			MUL
			STA 0F0H;r0
			PSHX
			LDA 0A0H
			LDX 0A3H
			MUL
			AIS #0FFH
			ADD 0,SP
			PSHA
			PSHX
			LDA 0A1H
			LDX 0A2H
			MUL
			AIS #0FEH
			ADC 0,SP
			STA 0F1;r1
			
			TXA
			AIS #01H
			ADC 0,SP
			PSHA
			
			LDA 0A0H
			LDX 0A2H
			MUL
			AIS 0FFH
			ADC 0,SP
			STA 0F2H;R2
			
			TXA
			STA 0F3
			RTS
******************************************************************************************************
			
			
almacena:   ;compara y almacena
			
			
			DBNZ 60H, ciclo_k
			DBNZ 61H, ciclo_j
			DBNZ 62H, ciclo_i 
			feed_watchdog
fin:        BRA    fin
			

