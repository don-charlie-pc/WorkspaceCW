;*******************************************************************
;* This stationery serves as the framework for a user application. *
;* For a more comprehensive program that demonstrates the more     *
;* advanced functionality of this processor, please see the        *
;* demonstration applications, located in the examples             *
;* subdirectory of the "Freescale CodeWarrior for HC08" program    *
;* directory.                                                      *
;*******************************************************************
;REFERENCIA
;http://www.todopic.com.ar/foros/index.php?topic=16371.0

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
			
			MOV #03FH,PTBDD
			;PTBD 3 a 0 => 7 a 4
			;PTBD 4 => E
			;PTBD 5 => RS
					
mainLoop:

inicio:
			JSR delay_10ms
			JSR delay_10ms
			
			
			MOV #03H, PTBD
			BSET 4,PTBD ;Enable On
			JSR delay_100us
			BCLR 4,PTBD ;Enable Off
			JSR delay_5ms
			
			MOV #03H, PTBD
			BSET 4,PTBD ;Enable On
			JSR delay_100us
			BCLR 4,PTBD ;Enable Off
			JSR delay_5ms
			
			MOV #03H, PTBD
			BSET 4,PTBD ;Enable On
			JSR delay_100us
			BCLR 4,PTBD ;Enable Off
			JSR delay_5ms
			
			MOV #03H, PTBD
			BSET 4,PTBD ;Enable On
			JSR delay_100us
			BCLR 4,PTBD ;Enable Off
			JSR delay_5ms

			MOV #02H, PTBD
			BSET 4,PTBD ;Enable On
			JSR delay_100us
			BCLR 4,PTBD ;Enable Off
			JSR delay_5ms
			
			LDA #28H
    		JSR send_cmd
    		LDA #0Ch
		    JSR send_cmd
		    LDA #06h
		    JSR send_cmd
		    LDA #01h
		    JSR send_cmd
		    
		    LDA #'H'
    		JSR send_data
    		LDA #'O'
		    JSR send_data
		    LDA #'L'
		    JSR send_data
		    LDA #'A'
		    JSR send_data
		    
aqui:     	feed_watchdog
			BRA aqui
			
send_data:	
			PSHA
			AND #0F0H ;Primer nibble
			NSA
			STA PTBD
			BSET 5,PTBD ;Para datos
			BSET 4,PTBD ;Enable On
			JSR delay_100us
			BCLR 4,PTBD ;Enable Off
			JSR delay_100us
			
			PULA ;Segundo nibble
			AND #0FH
			STA PTBD
			BSET 5,PTBD ;Para datos
			BSET 4,PTBD ;Enable On
			JSR delay_100us
			BCLR 4,PTBD ;Enable Off
			JSR delay_100us
			RTS	
			
send_cmd:	
			PSHA
			AND #0F0H ;Primer nibble
			NSA
			STA PTBD
			BSET 4,PTBD ;Enable On
			JSR delay_100us
			BCLR 4,PTBD ;Enable Off
			JSR delay_5ms
			
			PULA ;Segundo nibble
			AND #0FH
			STA PTBD
			BSET 4,PTBD ;Enable On
			JSR delay_100us
			BCLR 4,PTBD ;Enable Off
			JSR delay_5ms
			RTS			

			;Retardo de 10ms con CLK => 4 MHz			
delay_10ms: LDX #40
loop1_10ms:	LDA #250
loop2_10ms:	feed_watchdog
			DBNZA loop2_10ms ;4 ciclos => 1 us
        	DBNZX loop1_10ms ;4 ciclos => 1 us
        	RTS

;Retardo de 5ms con CLK => 4 MHz			
delay_5ms:  LDX #40
loop1_5ms:	LDA #250
loop2_5ms:	feed_watchdog
			DBNZA loop2_5ms ;4 ciclos => 1 us
        	DBNZX loop1_5ms ;4 ciclos => 1 us
        	RTS

			;Retardo de 100us
delay_100us:LDX #100		
loop_100us:	feed_watchdog
			DBNZX loop_100us
			RTS
