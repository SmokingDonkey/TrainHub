/***********************************************************************
 * $Id:: main.c 1604 2012-04-24 11:34:47Z nxp31103     $
 *
 * Project: CANopen Application Example for LPC11Cxx (master node)
 *
 * Description:
 *   CANopen example using on-chip CANopen drivers
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
 **********************************************************************/

#include <stdio.h>
#include <string.h>
#include "LPC11xx.h"
#include "canopen_driver.h"
#include "CAN_Node_Def.h"

/* interrupts */
void TIMER16_0_IRQHandler(void);

/* CAN Callback function prototypes */
void CANopen_Init_SDO(void);								/* callback function for setting initial SDO */
void CANopen_Heartbeat_Consumer_Failed(uint8_t Node_ID);	/* callback function for heartbeat consumer did not send heartbeat in time */
void CANopen_NMT_Reset_Node_Received(void);					/* callback function for reacting to NMT command Reset Node  */
void CANopen_NMT_Reset_Comm_Received(void);					/* callback function for reacting to NMT command Reset Communication */

/* Global Variables */
volatile uint8_t SimulateHeartbeatFail;						/* used for simulating a heartbeat failure */

/*****************************************************************************
** Function name:		main
**
** Description:			C entry. Initializes HW and starts application code
**
** Parameters:			None
** Returned value:		None
*****************************************************************************/
int main(void)
{
	SystemInit();
	CANopenInit();

	/* Timer16B0 */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);		/* Enable clock for 16-bit counter/timer 0 */
	LPC_TMR16B0->MCR = 3;						/* Interrupt and Reset on MR0 */
	LPC_TMR16B0->MR0 = SystemCoreClock/1000;	/* Interrupt and Reset on MR0 */
    NVIC_EnableIRQ(TIMER_16_0_IRQn);			/* Enable the Timer0 Interrupt */
    LPC_TMR16B0->TCR = 1;						/* Enable Timer0 */

    /*LEDArray */
    LPC_GPIO2->DIR |= 0x000000FF;				/* LEDs as output */

    LEDArray = 0x00;

	while(1)
	{
		__WFI();	/* Go to Sleep */
	}
	return 0 ;
}

/*****************************************************************************
** Function name:		TIMER16_0_IRQHandler
**
** Description:			Set to fire every 1ms
**
** Parameters:			None
** Returned value:		None
*****************************************************************************/
void TIMER16_0_IRQHandler(void)
{
	static uint32_t PrevButtons;

	LPC_TMR16B0->IR = 1;			/* clear interrupt flag */

	/* CANopen 1ms tick */
	CANopen_1ms_tick();

	/* button handler */
    if(!(LPC_GPIO0->DATA & (1<<1)) && (PrevButtons & (1<<1)))						/* if button pressed now, and not pressed previous iteration */
    {
    	SimulateHeartbeatFail = 1;
    }
    if(!(LPC_GPIO1->DATA & (1<<4)) && (PrevButtons & (1<<4)))						/* if button pressed now, and not pressed previous iteration */
    {
    	LEDArray--;
    }
    if(!(LPC_GPIO1->DATA & (1<<5)) && (PrevButtons & (1<<5)))						/* if button pressed now, and not pressed previous iteration */
    {
    	LEDArray++;
    }

    /* save button state */
	PrevButtons =  (LPC_GPIO0->DATA & (1<<1)) | (LPC_GPIO1->DATA & (1<<4 | 1<<5));

	/* in case of having to simulate a heartbeat failure, reset the producer counter every 1ms */
    if(SimulateHeartbeatFail)
    	CANopen_Heartbeat_Producer_Counter = 0;

    /* display value of LEDArray */
	LPC_GPIO2->DATA &= ~0x000000FF;
	LPC_GPIO2->DATA |= (LEDArray & 0xFF);
}

/*****************************************************************************
** Function name:		CANopen_Init_SDO
**
** Description:			Callback function for initializing SDO parameters
**
** Parameters:			None
** Returned value:		None
*****************************************************************************/
void CANopen_Init_SDO(void)
{
	uint32_t i;

	LEDArray = 0;
	error_register = 0;
	for(i=0; i<WatchListLength; i++)
	{
		WatchList[i].value = 0;
		WatchList[i].counter = 0;
	}
	CANopen_Heartbeat_Producer_Value = 0;
	CANopen_Heartbeat_Producer_Counter = 0;
	SimulateHeartbeatFail = 0;
}

/*****************************************************************************
** Function name:		NMT_Reset_Node
**
** Description:			NMT Reset Node callback.
** 						Called upon receiving reset node NMT command.
** 						Can be added by user
**
** Parameters:			None
** Returned value:		None
*****************************************************************************/
void CANopen_NMT_Reset_Node_Received(void)
{

}

/*****************************************************************************
** Function name:		NMT_Reset_Comm
**
** Description:			NMT Reset Communication.
** 						Called upon CanInit / receiving reset node/communication NMT command.
**
** Parameters:			None
** Returned value:		None
*****************************************************************************/
void CANopen_NMT_Reset_Comm_Received(void)
{

}

/*****************************************************************************
** Function name:		CAN_Heartbeat_Fail
**
** Description:			CAN heartbeat fail callback.
** 						Function is executed when a heartbeat producer which
** 						is listed in the heartbeat consumer list did not
** 						send a heartbeat on time. Callback functions is executed
** 						after sending the NMT reset command.
**
** Parameters:			Node_ID. Contains the number of the Node_ID which did
** 						not respond on time.
** Returned value:		None
*****************************************************************************/
void CANopen_Heartbeat_Consumer_Failed(uint8_t Node_ID)
{

}

/*****************************************************************************
** Function name:		CANopen_NMT_Consumer_Bootup_Received
**
** Description:			CAN consumer bootup received callback.
** 						Function is executed when a node which is listed in the
** 						watchlist send a boot-up message.
**
** Parameters:			Node_ID. Contains the number of the Node_ID which send
** 						the boot-up message
** Returned value:		None
*****************************************************************************/
void CANopen_NMT_Consumer_Bootup_Received(uint8_t Node_ID)
{

}

/******************************************************************************
**                            End Of File
******************************************************************************/
