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
;############################################################################################			

			;Configurar puerto
			MOV #0F0H,PTBDD
			;PTBD bits 4 al 7: LEDS
			;PTBD bit 2: PUSH_BUTTON
			
			;64H bit 0: Indica si es sencillo o doble
			CLR 64H
			
			;63H bit0 = 0. Iniciar con secuencia 1.
			CLR 63H
			
			LDX #04
			LDA #08H
			  
empezar:    
			;Determinar qué secuencia ejecutar
			
			;Caso derecha
			BRCLR 2,63H,caso_izq	
			BRSET 0,64H,der_doble
			;Derecha sencillo
			BRA derecha
			
			;Caso izquierda
caso_izq:	BRSET 0,64H,izq_doble
			;Izquierdo sencillo
			BRA izquierda			
			
			
			
derecha:	BCLR 0,64H
ciclo_der:	LSRA
			STA PTBD ;Cargar barrido a puerto
			AIS #01H
			JSR delay
			AIS #-01H
			
der_doble:	BSET 0,64H
			PSHA
			CBEQA #10H,limite_der
			LSRA
			BRA sigue_der
limite_der: LDA #10H			
sigue_der:	AIS #-01H
			ORA 0,SP
			STA PTBD
			AIS #01H
			JSR delay
			AIS #-01H
			AND 0,SP
			DBNZX ciclo_der
			LDX #04
			SEC
			BRA derecha

izquierda:	
			BCLR 0,64H
ciclo_izq:	ASLA
			STA PTBD ;Cargar barrido a puerto
			JSR delay
			
			
izq_doble:	BSET 0,64H
			PSHA
			CBEQA #80H,limite_izq
			ASLA
			BRA sigue_izq
limite_izq: LDA #10H			
sigue_izq:	AIS #01H
			ORA 0,SP
			STA PTBD
			AIS #-01H
			JSR delay
			AIS #01H
			AND 0,SP
			DBNZX ciclo_izq
			LDX #04
			LDA #08H
			BRA izquierda

;delay---------------------------------------
delay:		
			;Tres ciclos anidados para delay			
			MOV #08H,62H
loop2:		MOV #0FFH,61H
loop1:		MOV #0FFH,60H
loop0:		NOP
			DBNZ 60H,loop0
			feed_watchdog
			DBNZ 61H,loop1
			DBNZ 62H,loop2
			
			;Checar si el push button está presionado
			BRCLR 2,PTBD,continuar
			AIS #01H ;Ajusta SP porque no se ejecutará RTS
			;Complementar 63H bit0 para indicar la nueva secuencia a ejecutar
			BRCLR 2,63H,prende_bit
			CLR 63H
			JMP empezar ;Iniciar nueva secuencia
prende_bit:	MOV #04H,63H
			JMP empezar	;Iniciar nueva secuencia
			
continuar:	RTS ;Retorno de subrutina
