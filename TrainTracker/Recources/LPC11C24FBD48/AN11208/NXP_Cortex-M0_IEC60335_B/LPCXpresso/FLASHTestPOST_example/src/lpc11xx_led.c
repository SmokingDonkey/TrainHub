;/****************************************************************************
; *   $Id:: lpc11xx_led.c 8707 2011-11-24 07:57:47Z nxp28536                 $
; *   Project: LPC1100 LPCXpresso LED driver
; *
; *   Description:
; *     Main source file
; *	    
; *
; ****************************************************************************
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
;****************************************************************************/
#include "LPC11xx.h"
#include "lpc11xx_led.h"

void initLed (uint8_t led)
{
	LED_GPIO->DIR |= SET_BIT(led);
}

void toggleLed (uint8_t led)
{
	if(LED_GPIO->DATA & SET_BIT(led)) { clearLed(led); }
		else setLed(led);
}


