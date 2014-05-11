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
            ;Ordenar 8 datos desde 60H con MergeSort
            MOV #60H,0D0H ;s
            MOV #67H,0D1H ;f
            
            ;Apuntadores de arreglos merge
            CLR 0D2H ;i
            CLR 0D3H ;j
            
            ;Apuntador para cache
            CLR 0D4H ;k
            
            ;Contador para merge
            CLR 0D5H ;count
            
            CLRH
            
            JSR merge_sort
fin:		BRA fin
            
            
merge_sort: ;Media
            LDA 0D0H
            ADD 0D1H
            LSRA
            TAX ;Guardo m en X
            
            LDA 0D0H
            CMP 0D1H
            BHI regresa ;Condicion de paro
            
       		;merge_sort(s,m)------------------
       		;Respaldar m, s y f
       		LDA 0D0H ;Meter s
       		PSHA
       		LDA 0D1H ;Meter f
       		PSHA
       		PSHX ;Meter m
       		
       		;Cargar nuevos valores a subrutina
       		TXA
       		STA 061H
       		JSR merge_sort
       		
       		;merge_sort(m+1,f)------------------
       		PULX ;Sacar m
       		INCX
       		STX 060H
       		
       		PULA ;Sacar f
       		STA 061H
       		
       		PSHA ;Meter f
       		PSHX ;Meter m
       		JSR merge_sort
       		
       		;Ejecuta merge----------------------
       		PULX ;Sacar m
       		PULA ;Sacar f
       		STA 0D1H
       		PULA ;Sacar s
       		STA 0D0H
       		JSR merge
       		
            feed_watchdog          
regresa:    RTS
            

           ;Crear cache desde 70H hasta 77H
merge:		LDA 0D1H
			SUB 0D0H
			INCA
			STA 0D5H
			
			;Ajustar i en inicio de arreglo
			LDA 0D0H
			STA 0D2H
			
			;Ajustar j en m+1 de arreglo
			PSHX
			AIS #01H
			ADD 0,SP
			STA 0D3H
			
			;Ajustar k en inicio de cache
			MOV #70H,0D4H		
			
ciclo:		LDX 0D2H ;m ya no lo necesito
			LDA ,X
			LDX 0D3H
			CMP ,X
			
			BHI carga_j
			;Carga i
			LDX	0D2H
			STA ,X
			INCX
			STX	0D2H ;i++
			LDX 064H
			STA ,X
			BRA inc_k
			
carga_j:	;Carga k
			LDX	0D3H
			STA ,X
			INCX
			STX	0D3H ;j++
			LDX 064H
			STA ,X
			
inc_k:		LDA 0D4H
			INCA
			STA 0D4H
			DBNZ 0D5H,ciclo
			
			;Reinicia i
			MOV 0D2H,0D0H
			;Reinicia k
			MOV #70H,0D4H
			;Reinicia count
			LDA 0D1H
			SUB 0D0H
			INCA
			STA 0D5H
			
			;Copiar arreglo
ciclo2:		LDX 0D2H
			LDA ,X
			LDX 0D4H
			STA ,X
			INC 0D2H
			INC 0D4H
			DBNZ 0D5H,ciclo2

			RTS


