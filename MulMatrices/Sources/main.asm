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

mainLoop:
            ;Apuntador A
            MOV #01H,63H
            CLR 64H
            ;Apuntador C
            MOV #01H,67H
            MOV #0FH,68H 
            
            MOV #03,62H ;i
            
loop3:      ;Apuntador B
            MOV #01H,65H
            MOV #09H,66H
            MOV #02,61H ;j
            
loop2:      CLR 69H ;sumh
			CLR 69H ;suml
            MOV #03,60H ;k

loop1:		STHX 63H
			LDA ,X ;Leer dato1
			PSHA
			STHX 65H
			LDA ,X ;Leer dato2
			TAX
			PULA
			MUL
			;EN 69H y 70H se guarda la suma
			;Respaldar suma------
			ADD 70H
			STA 70H 
			TXA
			ADC 69H
			STA 69H
			;--------------------
          
          	;Ajustar apuntador A (mas 1)
          	LDHX 63H
          	AIX #01H
          	STHX 63H
          	;Ajustar apuntador B (mas 2)
			LDHX 65H
          	AIX #02H
          	STHX 65H         
          
            DBNZ 60H,loop1
            
            ;--------------------
            LDHX 67H;Guardar RESH
            LDA 69H
            STA ,X
            AIX #01H
            LDA 70H
            STA ,X
            ;--------------------
            
            ;Ajustar apuntador A (menos 3)
          	LDHX 63H
          	AIX #-03H
          	STHX 63H
          	;Ajustar apuntador B (menos 5)
          	LDHX 65H
          	AIX #-05H
          	STHX 65H
          	;Ajustar apuntador C (mas 2)
          	LDHX 67H
          	AIX #02H
          	STHX 67H
            
            DBNZ 61H,loop2
            
            ;Ajustar apuntador A (mas 3)
          	LDHX 63H
          	AIX #03H
          	STHX 63H
          	
          	DBNZ 62H,loop3

 here:      feed_watchdog
            BRA    here


