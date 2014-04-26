/**********************************************************************
* $Id$		nxp28536			2012-06-19
*//**
* @file		keil_lpc1769_mcb.h
* @brief	configuration specific settings for Keil MCB1769 board 
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
#ifndef __KEIL_1769_MCB__H
#define __KEIL_1769_MCB__H

#define LED_PORT	1
#define LED_BLINK	(1u<<28)
#define LED_OK		(1u<<29)
#define LED_ERROR	(1u<<31)



#define enableLed(n)	LPC_GPIO1->FIODIR |= n
#define setLed(n) 		LPC_GPIO1->FIOSET = n
#define clearLed(n)		LPC_GPIO1->FIOCLR = n

#define toggleLed(n) do {\
	if(LPC_GPIO1->FIOPIN & n) {\
		LPC_GPIO1->FIOCLR = n;\
	} else {\
		LPC_GPIO1->FIOSET = n;\
	};\
} while(0);

static void setupLeds(void) {
	
	clearLed(LED_BLINK);
	clearLed(LED_OK);
	clearLed(LED_ERROR);
	enableLed(LED_BLINK);
	enableLed(LED_OK);
	enableLed(LED_ERROR);	
	
	// clear the rest
	LPC_GPIO2->FIOCLR = (0x1F << 2);
	LPC_GPIO2->FIODIR |= (0x1F << 2);
	

}


static void setVisualError(void) {

	clearLed(LED_OK);
	setLed(LED_ERROR); 
	
}

#endif

