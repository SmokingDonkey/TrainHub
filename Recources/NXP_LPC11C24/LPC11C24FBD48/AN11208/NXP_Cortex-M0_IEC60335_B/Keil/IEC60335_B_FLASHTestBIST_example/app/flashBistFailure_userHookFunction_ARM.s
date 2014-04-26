;/**
; * @file	cpuPostFailure_userHookFunctions_IAR.s
; * @purpose	definition of user hook function for CLASS B lib test
; * @version	v1.0
; * @date		11-may-2011
; * @author		nxp28536
;*/
;/*----------------------------------------------------------------------------
; * Software that is described herein is for illustrative purposes only
; * which provides customers with programming information regarding the
; * products. This software is supplied "AS IS" without any warranties.
; * NXP Semiconductors assumes no responsibility or liability for the
; * use of the software, conveys no license or title under any patent,
; * copyright, or mask work right to the product. NXP Semiconductors
; * reserves the right to make changes in the software without
; * notification. NXP Semiconductors also make no representation or
; * warranty that such application will be suitable for the specified
; * use without further testing or modification.
; **********************************************************************/
                
        EXPORT  _flashBistTestFailureHook
                
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; User hook function in case CPU reg test fails
;;

PIOxy_FAULT_DIR    EQU  (0x1 << 7)    
PIOxy_FAULT_DIRADD EQU (0x50008000)    
PIOxy_FAULT_SETADD EQU (0x50000008 :OR: PIOxy_FAULT_DIR) 
PIOxy_FAULT_SET    EQU (0x1 << 7) 
		
		AREA    |.text|, CODE, READONLY
		THUMB       

	

_flashBistTestFailureHook

;------------------------------------------
; Try to write a fault status over a selected port
;------------------------------------------
  ldr   r1,=PIOxy_FAULT_DIRADD  ; set the GPIO port as output
  ldr   r0,=PIOxy_FAULT_DIR
  str   r0, [r1]

  ldr   r1,=PIOxy_FAULT_SETADD  ; set the GPIO output signal
  ldr   r0,=PIOxy_FAULT_SET
  str   r0, [r1]

_haltCPUnow
  b _haltCPUnow                 ; now keep looping forever
  
		ALIGN
        END
