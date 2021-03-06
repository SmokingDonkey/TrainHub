/**********************************************************************
* $Id$		sbl.h			2012-05-04
*//**
* @file		sbl.h	
* @brief		Contain API for SBL transmission.
* @version	1.0
* @date		04. May. 2012
* @author	NXP MCU SW Application Team
* 
* Copyright(C) 2011, NXP Semiconductor
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

#ifndef __SBL_H__
#define __SBL_H__
#include "lpc_types.h"

/********Definitions ***********/

/** Memory Map */
#define SBL_FW_DATA_MAX_BLOCK_NUM	(512)
#define SBL_FW_DATA_BLOCK_SIZE		(0x400)
#define SBL_SECTOR0_BLOCK_SIZE		(4)

/** Function ID */

/**  Firmware Version ID Function */
#define SBL_ReadFirmVerionID_FnID	(0x31)
#define SBL_FwVersionID_ParamSize	(3)
#define SBL_FwVersionID_Major_Idx 	(0)
#define SBL_FwVersionID_Minor_Idx	(1)
#define SBL_FwVersionID_Rev_Idx		(2)

/** Upgrade Firmware Function */
#define SBL_FwUpgrade_FnID			(0x32) 
#define SBL_FwUpgrade_ParamSize	    (SBL_FW_DATA_BLOCK_SIZE+3)
#define SBL_FwUpgrade_MSB_Idx       (0)
#define SBL_FwUpgrade_LSB_Idx       (1)
#define SBL_FwUpgrade_Fw_Idx        (2)
#define SBL_FwUpgrade_Checksum_Idx  (SBL_FwUpgrade_Fw_Idx+SBL_FW_DATA_BLOCK_SIZE) 
#define SBL_FwUpgrade_Checksum_Done_Idx  (2) 

#define SBL_Status_ParamSize		(3)
#define SBL_Status_OK				(0x55)
#define SBL_Status_ERR				(0xFF)

/** Sector 0 programming */
#define SBL_Sector0Upgrade_FnID		(0x33)
#define SBL_Sector0Upgrade_ParamSize	(SBL_FW_DATA_BLOCK_SIZE+4)
#define SBL_Sector0Upgrade_FnID_Idx      (0)
#define SBL_Sector0Upgrade_MSB_Idx       (1)
#define SBL_Sector0Upgrade_LSB_Idx       (2)
#define SBL_Sector0Upgrade_Data_Idx		 (3)
#define SBL_Sector0Upgrade_Checksum_Idx  (SBL_Sector0Upgrade_Data_Idx+SBL_FW_DATA_BLOCK_SIZE)
#define SBL_Sector0Upgrade_Checksum_Done_Idx  (3) 

/** SBL ready */
#define SBL_READY_BYTE				(0xAA)

/** SBL Return Code Values */
/* Function runs ok */
#define SBL_OK					(0x0000)	
/* All firmware data is received */
#define SBL_FW_UPGRADE_END		(0x0001)
/* All Sector0 data is received */
#define SBL_SECTOR0_UPGRADE_END  (0x0002)
/* There is an error when running the function */
#define SBL_ERR					(0xFFFF)
/* The input parameter values are invalid */
#define SBL_INVALID_PARAM 		(0x1001)
/* Communication error: ACK isn't received */
#define	SBL_COMM_ERR			(0x1002)
/* Slave has error when writing to flash memory */
#define SBL_FLASH_WRITE_ERR			(0x1004)
/* The Command ID in Upgrade Firmware function is incorrect */
#define SBL_INVALID_CMD_ID	(0x1008)
/* Check sum is incorrect */
#define SBL_CHECKSUM_ERR		(0x1010)
/* Timeout error */
#define SBL_TIMEOUT_ERR		    (0x1020)
/* Function ID is incorrect */
#define SBL_FN_ID_ERR           (0x1040)

#define SBL_RETRIES_MAX_NUM		(3)
#define SBL_TIMEOUT             (2000)          // about 2s

/********Data Types ***********/
/** SBL Function ID Type */
typedef uint8_t SBL_FUNC_ID;
/** SBL Status Type */
typedef uint8_t SBL_STATUS;
/** SBL return code Type */
typedef int16_t SBL_RET_CODE;

/* Package Type */
typedef enum
{
	START_PACKET,
	MID_PACKET,
	STOP_PACKET,
	START_STOP_PACKET,
}PACKET_TYPE;
/** SBL Transfer Information */
typedef struct
{
	uint8_t* TxBuf;			// pointer to transmit buffer
	uint8_t* RxBuf;			// Pointer to receive buffer
	uint32_t TxLen;			// Size of transmit buffer
	uint32_t RxLen;			// Size of receive buffer
	PACKET_TYPE Type;		// The type of packet
	
} SBL_TRANSER_Type, *PSBL_TRANSER_Type;

/** Firmware Version ID structure */
typedef struct
{
	uint8_t Major;			// major
	uint8_t Minor;			// minor
	uint8_t Revision;		// revision
} SBL_FirmVerion_Type, *PSBL_FirmVerion_Type;

/********Public Functions***********/
/* Initilize SBL Device */
SBL_RET_CODE SBL_Init(void);
/* De-Initilize SBL Device */
SBL_RET_CODE SBL_DeInit(void);
/* Read/Write data*/
SBL_RET_CODE SBL_ReadWrite(PSBL_TRANSER_Type pBuffer);
/* 	Check if it is able to send data */
Bool SBL_TxReady(void);
/* Check if there is data for receiving*/
Bool SBL_RxReady(void);
/* Send a byte */
void SBL_SendByte(uint8_t data);
/* Start to receive */
void SBL_StartReceive(void);
/* Receive a byte */
uint8_t SBL_ReceiveByte(void);


#endif /*__SBL_H__*/

