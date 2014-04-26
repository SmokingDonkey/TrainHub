/***********************************************************************
 * $Id:: main.c 1604 2012-04-24 11:34:47Z nxp31103     $
 *
 * Project: CANopen Application Example for LPC11Cxx (slave node)
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
#include "uart.h"
#include "canopen_driver.h"
#include "CAN_Node_Def.h"

#define HEARTBEAT_PRODUCER_TIME		4000		/* in ms */
#define HEARTBEAT_CONSUMER_TIME		4500		/* in ms */

/* interrupts */
void TIMER16_0_IRQHandler(void);

/* CAN Callback function prototypes */
void CANopen_Init_SDO(void);								/* callback function for setting initial SDO */
void CANopen_Heartbeat_Consumer_Failed(uint8_t Node_ID);	/* callback function for heartbeat consumer did not send heartbeat in time */
void CANopen_NMT_Reset_Node_Received(void);					/* callback function for reacting to NMT command Reset Node  */
void CANopen_NMT_Reset_Comm_Received(void);					/* callback function for reacting to NMT command Reset Communication */

/* used by application */
void Print_SDO_SEG_Entry(uint8_t node_id, uint16_t index, uint8_t subindex);	/* prints SDO segmented entry */
void Print_SDO_EXP_Entry(uint8_t node_id, uint16_t index, uint8_t subindex);	/* prints SDO expedited entry */

/* Global Variables */
uint8_t LEDArray;			/* digital representation of the LEDs, updated every 1ms */
char buffer[255];			/* general purpose buffer for application */
uint32_t FakedCMD;			/* Used for faking a terminal command by pushbuttons */

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
	uint32_t CMD;
	uint32_t i;
	uint8_t err;

	SystemInit();
	CANopenInit();
	UART_Init(115200);

	/* Timer16B0 */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);		/* Enable clock for 16-bit counter/timer 0 */
	LPC_TMR16B0->MCR = 3;						/* Interrupt and Reset on MR0 */
	LPC_TMR16B0->MR0 = SystemCoreClock/1000;	/* Interrupt and Reset on MR0 */
    NVIC_EnableIRQ(TIMER_16_0_IRQn);			/* Enable the Timer0 Interrupt */
    LPC_TMR16B0->TCR = 1;						/* Enable Timer0 */

    /* LEDArray */
    LPC_GPIO2->DIR |= 0x000000FF;				/* LEDs as output */

    LEDArray = 0x00;
    WatchList[0].NodeID = CAN_SLAVE1_NODE;					/* Slave node 1 in watchlist */
    WatchList[0].ConsumerTime = HEARTBEAT_CONSUMER_TIME;	/* Consumer time for watch node 1. This will restore the consumer heartbeat when the slave node resets after heartbeat failure */
    WatchList[0].ProducerTime = HEARTBEAT_PRODUCER_TIME;	/* Producer time for watch node 1. This will restore the producer heartbeat when the slave node resets after heartbeat failure */
    WatchList[1].NodeID = CAN_SLAVE2_NODE;
    WatchList[1].ConsumerTime = HEARTBEAT_CONSUMER_TIME;	/* Consumer time for watch node 2. This will restore the consumer heartbeat when the slave node resets after heartbeat failure */
    WatchList[1].ProducerTime = HEARTBEAT_PRODUCER_TIME;	/* Producer time for watch node 2. This will restore the producer heartbeat when the slave node resets after heartbeat failure */

    printf("\r\n");
    printf("********************************************************************************\r\n");
    printf("*                                                                              *\r\n");
    printf("*                             CANopen Example V1.0                             *\r\n");
    printf("*                                                                              *\r\n");
    printf("********************************************************************************\r\n");

    while(1)
    {
		printf("\r\nPlease enter one of the following commands:\r\n");
		printf(" 1) Configure slave node 1.\r\n");
		printf(" 2) Configure slave node 2.\r\n");
		printf(" 3) Read SDO slave node 1.\r\n");
		printf(" 4) Read SDO slave node 2.\r\n");
		printf(" 5) Write to SDO entry 2200h of slave node 1.\r\n");
		printf(" 6) Write to SDO entry 2200h of slave node 2.\r\n");
		printf(" 7) Write to LEDS of slave node 1.\r\n");
		printf(" 8) Write to LEDS of slave node 2.\r\n");
		printf(" 9) Copy LEDs of slave node 1 to master node.\r\n");
		printf("10) Copy LEDs of slave node 2 to master node.\r\n");
		printf("11) Simulate heartbeat failure of master node.\r\n");
		printf("Command: ");

		do
		{
			fgets(buffer, sizeof(buffer), stdin);
			i = sscanf(buffer,"%d", &CMD);
		}
		while(i != 1 && (!StopReadingUART));

    	if(StopReadingUART)
    	{
    		StopReadingUART = 0;
    		CMD = FakedCMD;
    	}
		printf("\r\n\r\nExecuting command: %d)", CMD);

		err = 0;
		switch(CMD)
		{
		case 1:
			printf(" Configure slave node 1.\r\n");
			WatchList[0].value = 0x00;	/* Disable heartbeat checking while configuring */
			buffer[0] = (WatchList[0].ProducerTime >> 8) & 0xFF;
			buffer[1] = (WatchList[0].ProducerTime >> 0) & 0xFF;
			CANopen_SDOC_Exp_Write(CAN_SLAVE1_NODE, 0x1017, 0x00, (uint8_t*)buffer, 2);
			while(!(CANopen_SDOC_State == CANopen_SDOC_Succes || CANopen_SDOC_State == CANopen_SDOC_Fail))
				;
			if(CANopen_SDOC_State == CANopen_SDOC_Succes)
				WatchList[0].value = (WatchList[0].NodeID)<<16 | WatchList[0].ConsumerTime;	/* node 1 */
			else
				printf("Error: Failed while executing command!\r\n");

			buffer[0] = 0x00;
			buffer[1] = CAN_NODE_ID;
			buffer[2] = (WatchList[0].ConsumerTime >> 8) & 0xFF;
			buffer[3] = (WatchList[0].ConsumerTime >> 0) & 0xFF;
			CANopen_SDOC_Exp_Write(CAN_SLAVE1_NODE, 0x1016, 0x01, (uint8_t*)buffer, 4);
			while(!(CANopen_SDOC_State == CANopen_SDOC_Succes || CANopen_SDOC_State == CANopen_SDOC_Fail))
				;
			if(CANopen_SDOC_State == CANopen_SDOC_Fail)
				printf("Error: Failed while executing command!\r\n");
			break;
		case 2:
			printf(" Configure slave node 2.\r\n");
			WatchList[1].value = 0x00;	/* Disable heartbeat checking while configuring */
			buffer[0] = (WatchList[1].ProducerTime >> 8) & 0xFF;
			buffer[1] = (WatchList[1].ProducerTime >> 0) & 0xFF;
			CANopen_SDOC_Exp_Write(CAN_SLAVE2_NODE, 0x1017, 0x00, (uint8_t*)buffer, 2);
			while(!(CANopen_SDOC_State == CANopen_SDOC_Succes || CANopen_SDOC_State == CANopen_SDOC_Fail))
				;
			if(CANopen_SDOC_State == CANopen_SDOC_Succes)
				WatchList[1].value = (WatchList[1].NodeID)<<16 | WatchList[1].ConsumerTime;	/* node 2 */
			else
				printf("Error: Failed while executing command!\r\n");
			buffer[0] = 0x00;
			buffer[1] = CAN_NODE_ID;
			buffer[2] = (WatchList[1].ConsumerTime >> 8) & 0xFF;
			buffer[3] = (WatchList[1].ConsumerTime >> 0) & 0xFF;
			CANopen_SDOC_Exp_Write(CAN_SLAVE2_NODE, 0x1016, 0x01, (uint8_t*)buffer, 4);
			while(!(CANopen_SDOC_State == CANopen_SDOC_Succes || CANopen_SDOC_State == CANopen_SDOC_Fail))
				;
			if(CANopen_SDOC_State == CANopen_SDOC_Fail)
				printf("Error: Failed while executing command!\r\n");
			break;
		case 3:
			printf(" Read SDO slave node 1.\r\n");
			printf("Node ID\tIndex\tSubindex\tEXP/SEG\tSize\tData\r\n");
			Print_SDO_EXP_Entry(CAN_SLAVE1_NODE, 0x1000, 0x00);
			Print_SDO_EXP_Entry(CAN_SLAVE1_NODE, 0x1001, 0x00);
			Print_SDO_EXP_Entry(CAN_SLAVE1_NODE, 0x1016, 0x00);
			Print_SDO_EXP_Entry(CAN_SLAVE1_NODE, 0x1016, 0x01);
			Print_SDO_EXP_Entry(CAN_SLAVE1_NODE, 0x1017, 0x00);
			Print_SDO_EXP_Entry(CAN_SLAVE1_NODE, 0x1018, 0x00);
			Print_SDO_EXP_Entry(CAN_SLAVE1_NODE, 0x1018, 0x01);
			Print_SDO_EXP_Entry(CAN_SLAVE1_NODE, 0x2000, 0x00);
			Print_SDO_SEG_Entry(CAN_SLAVE1_NODE, 0x2200, 0x00);
			break;
		case 4:
			printf(" Read SDO slave node 2.\r\n");
			printf("Node ID\tIndex\tSubindex\tEXP/SEG\tSize\tData\r\n");
			Print_SDO_EXP_Entry(CAN_SLAVE2_NODE, 0x1000, 0x00);
			Print_SDO_EXP_Entry(CAN_SLAVE2_NODE, 0x1001, 0x00);
			Print_SDO_EXP_Entry(CAN_SLAVE2_NODE, 0x1016, 0x00);
			Print_SDO_EXP_Entry(CAN_SLAVE2_NODE, 0x1016, 0x01);
			Print_SDO_EXP_Entry(CAN_SLAVE2_NODE, 0x1017, 0x00);
			Print_SDO_EXP_Entry(CAN_SLAVE2_NODE, 0x1018, 0x00);
			Print_SDO_EXP_Entry(CAN_SLAVE2_NODE, 0x1018, 0x01);
			Print_SDO_EXP_Entry(CAN_SLAVE2_NODE, 0x2000, 0x00);
			Print_SDO_SEG_Entry(CAN_SLAVE2_NODE, 0x2200, 0x00);
			break;
		case 5:
			printf(" Write to SDO entry 2200h of slave node 1.\r\n");
			printf("Enter string to write to SDO entry 2200h of slave node 1:\r\n");

			fgets(buffer, sizeof(buffer), stdin);
			i = 0;
			while(buffer[i])
				i++;
			CANopen_SDOC_Seg_Write(CAN_SLAVE1_NODE, 0x2200, 0x00, buffer, i-1);
			while(!(CANopen_SDOC_State == CANopen_SDOC_Succes || CANopen_SDOC_State == CANopen_SDOC_Fail))
				;
			if(CANopen_SDOC_State == CANopen_SDOC_Fail)
				printf("\r\nError: Failed while executing command!\r\n");
			break;
		case 6:
			printf(" Write to SDO entry 2200h of slave node 2.\r\n");
			printf("Enter string to write to SDO entry 2200h of slave node 2:\r\n");

			fgets(buffer, sizeof(buffer), stdin);
			i = 0;
			while(buffer[i])
				i++;
			CANopen_SDOC_Seg_Write(CAN_SLAVE2_NODE, 0x2200, 0x00, buffer, i);
			while(!(CANopen_SDOC_State == CANopen_SDOC_Succes || CANopen_SDOC_State == CANopen_SDOC_Fail))
				;
			if(CANopen_SDOC_State == CANopen_SDOC_Fail)
				printf("Error: Failed while executing command!\r\n");
			break;
		case 7:
			printf(" Write to LEDS of slave node 1.\r\n");
			printf("Enter HEX number to write to LEDs:\r\n0x");
			fgets(buffer, sizeof(buffer), stdin);
			i = sscanf(buffer,"%X", (uint32_t*)&buffer[0]);
			CANopen_SDOC_Exp_Write(CAN_SLAVE1_NODE, 0x2000, 0x00, (uint8_t*)buffer, 1);
			while(!(CANopen_SDOC_State == CANopen_SDOC_Succes || CANopen_SDOC_State == CANopen_SDOC_Fail))
				;
			if(CANopen_SDOC_State == CANopen_SDOC_Fail)
				printf("Error: Failed while executing command!\r\n");
			break;
		case 8:
			printf(" Write to LEDS of slave node 2.\r\n");
			printf("Enter HEX number to write to LEDs:\r\n0x");
			fgets(buffer, sizeof(buffer), stdin);
			i = sscanf(buffer,"%X", (uint32_t*)&buffer[0]);
			CANopen_SDOC_Exp_Write(CAN_SLAVE2_NODE, 0x2000, 0x00, (uint8_t*)buffer, 1);
			while(!(CANopen_SDOC_State == CANopen_SDOC_Succes || CANopen_SDOC_State == CANopen_SDOC_Fail))
				;
			if(CANopen_SDOC_State == CANopen_SDOC_Fail)
				printf("Error: Failed while executing command!\r\n");
			break;
		case 9:
			printf(" Copy LEDs of slave node 1 to master node.\r\n");
			CANopen_SDOC_Exp_Read(CAN_SLAVE1_NODE, 0x2000, 0x00, (uint8_t*)buffer, NULL);
			while(!(CANopen_SDOC_State == CANopen_SDOC_Succes || CANopen_SDOC_State == CANopen_SDOC_Fail))
				;
			if(CANopen_SDOC_State == CANopen_SDOC_Succes)
				LEDArray = buffer[0];
			else
				printf("Error: Failed while executing command!\r\n");
			break;
		case 10:
			printf(" Copy LEDs of slave node 2 to master node.\r\n");
			CANopen_SDOC_Exp_Read(CAN_SLAVE2_NODE, 0x2000, 0x00, (uint8_t*)buffer, NULL);
			while(!(CANopen_SDOC_State == CANopen_SDOC_Succes || CANopen_SDOC_State == CANopen_SDOC_Fail))
				;
			if(CANopen_SDOC_State == CANopen_SDOC_Succes)
				LEDArray = buffer[0];
			else
				printf("Error: Failed while executing command!\r\n");
			break;
		case 11:
			printf(" Simulate heartbeat failure of master node.\r\n");
			CANopen_Heartbeat_Producer_Value = 0;
			printf("Master node has stopped sending its heartbeat.\r\n");
			break;
		default:
			printf(" Error: Unknown command!\r\n");
			err = 1;
			break;
		}

		if(!err)
			printf("\r\nFinished executing command %d)\r\n", CMD);

		printf("\r\n********************************************************************************\r\n");
	}
	return 0;
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

	/* button handler */
    if(!(LPC_GPIO0->DATA & (1<<1)) && (PrevButtons & (1<<1)))						/* if button pressed now, and not pressed previous iteration */
    {
    	CANopen_Heartbeat_Producer_Value = 0;
    }
    if(!(LPC_GPIO1->DATA & (1<<4)) && (PrevButtons & (1<<4)))						/* if button pressed now, and not pressed previous iteration */
    {
    	FakedCMD = 9;
    	StopReadingUART = 1;
    }
    if(!(LPC_GPIO1->DATA & (1<<5)) && (PrevButtons & (1<<5)))						/* if button pressed now, and not pressed previous iteration */
    {
    	FakedCMD = 10;
    	StopReadingUART = 1;
    }

    /* save button state */
	PrevButtons =  (LPC_GPIO0->DATA & (1<<1)) | (LPC_GPIO1->DATA & (1<<4 | 1<<5));

	/* CANopen 1ms tick */
	CANopen_1ms_tick();

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

	Error_Register = 0;
	for(i=0; i<WatchListLength; i++)
	{
		WatchList[i].value = 0;
		WatchList[i].counter = 0;
	}
	CANopen_Heartbeat_Producer_Value = HEARTBEAT_PRODUCER_TIME;
	CANopen_Heartbeat_Producer_Counter = 0;
}

/*****************************************************************************
** Function name:		CANopen_NMT_Reset_Node_Received
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
	printf("\r\n\tNotify: Master received NMT command \"Reset Node\".\r\n");
}

/*****************************************************************************
** Function name:		CANopen_NMT_Reset_Comm_Received
**
** Description:			NMT Reset Communication.
** 						Called upon CanInit / receiving reset node/communication NMT command.
**
** Parameters:			None
** Returned value:		None
*****************************************************************************/
void CANopen_NMT_Reset_Comm_Received(void)
{
	printf("\r\n\tNotify: Master received NMT command \"Reset Communication\".\r\n");
}

/*****************************************************************************
** Function name:		CANopen_Heartbeat_Consumer_Failed
**
** Description:			CAN heartbeat fail callback.
** 						Function is executed when a heartbeat producer which
** 						is listed in the watchlist did not
** 						send a heartbeat on time. Callback functions is executed
** 						after sending the NMT reset command.
**
** Parameters:			Node_ID. Contains the number of the Node_ID which did
** 						not respond on time.
** Returned value:		None
*****************************************************************************/
void CANopen_Heartbeat_Consumer_Failed(uint8_t Node_ID)
{
	printf("\r\n\tNotify: Did not receive heartbeat on time of node 0x%02X. Trying to reset node 0x%02X...\r\n", Node_ID, Node_ID);
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
	printf("\r\n\tNotify: Received bootup from node 0x%02X.\r\n", Node_ID);
}

/*****************************************************************************
** Function name:		Print_SDO_SEG_Entry
**
** Description:			used for printing a segmented SDO entry
**
** Parameters:			node_id: node-id of node to read.
** 						index: index to read
** 						subindex: subindex to read
** Returned value:		None
*****************************************************************************/
void Print_SDO_SEG_Entry(uint8_t node_id, uint16_t index, uint8_t subindex)
{
	CANopen_SDOC_Seg_Read(node_id, index, subindex, buffer, sizeof(buffer));
	while(!(CANopen_SDOC_State == CANopen_SDOC_Succes || CANopen_SDOC_State == CANopen_SDOC_Fail))
		;
	if(CANopen_SDOC_State == CANopen_SDOC_Succes)
		printf("0x%02X\t%04XH\t%02XH\t\tSEG\t-\t%s\r\n", node_id, index, subindex, buffer);
	else
		printf("Error: Failed while executing command!\r\n");
}

/*****************************************************************************
** Function name:		Print_SDO_EXP_Entry
**
** Description:			used for printing a expedited SDO entry
**
** Parameters:			node_id: node-id of node to read.
** 						index: index to read
** 						subindex: subindex to read
** Returned value:		None
*****************************************************************************/
void Print_SDO_EXP_Entry(uint8_t node_id, uint16_t index, uint8_t subindex)
{
	uint8_t i;

	buffer[0] = 0x00;
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	CANopen_SDOC_Exp_Read(node_id, index, subindex, (uint8_t*)buffer, &i);
	while(!(CANopen_SDOC_State == CANopen_SDOC_Succes || CANopen_SDOC_State == CANopen_SDOC_Fail))
		;
	if(CANopen_SDOC_State == CANopen_SDOC_Succes)
		printf("0x%02X\t%04XH\t%02XH\t\tEXP\t%d\t0x%02X 0x%02X 0x%02X 0x%02X\r\n", node_id, index, subindex, i, buffer[0], buffer[1], buffer[2], buffer[3]);
	else
		printf("Error: Failed while executing command!\r\n");
}

/******************************************************************************
 **                            End Of File
 ******************************************************************************/
