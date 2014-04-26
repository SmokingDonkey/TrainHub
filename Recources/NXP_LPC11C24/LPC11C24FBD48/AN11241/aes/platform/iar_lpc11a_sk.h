/**********************************************************************
* $Id$		nxp28536			2012-06-19
*//**
* @file		iar_lpc11a_sk.h
* @brief	configuration specific settings for IAR LPC11A starter kit 
* @version	1.0
* @date		19. June. 2012
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2012, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/
#ifndef __IAR_LPC11A_SK__H
#define __IAR_LPC11A_SK__H

#define LED_PORT	0
#define LED_BLINK	(1<<16)
#define LED_OK		(1<<17)
#define LED_ERROR	(1<<18)

#define enableLed(n)	{ LPC_GPIO_PORT->DIR[LED_PORT] |= n;}
#define setLed(n) 		{ LPC_GPIO_PORT->CLR[LED_PORT] = n; }
#define clearLed(n) 	{ LPC_GPIO_PORT->SET[LED_PORT] = n; }
#define toggleLed(n) 	{ LPC_GPIO_PORT->NOT[LED_PORT] = n; }

static void setupLeds(void) {
	
	clearLed(LED_BLINK);
	clearLed(LED_OK);
	clearLed(LED_ERROR);
	enableLed(LED_BLINK);
	enableLed(LED_OK);
	enableLed(LED_ERROR);	

}

static void setVisualError(void) {

	clearLed(LED_BLINK);
	setLed(LED_ERROR); 
	
}

/* used to make sure there are enough wait states when switching to 48MHz later */
void flashInit48Mhz(void) {
	
	LPC_FMC->FLASHCFG = ((LPC_FMC->FLASHCFG & 0xFFFFFFFC) | 0x3);
}


#endif

