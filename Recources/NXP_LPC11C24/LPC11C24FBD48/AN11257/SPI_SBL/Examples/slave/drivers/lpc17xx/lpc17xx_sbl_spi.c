/**********************************************************************
* $Id$		lpc17xx_sbl_spi.c			2012-05-04
*//**
* @file		lpc17xx_sbl_spi.c	
* @brief	Contain API for SBL transmission using SPI (Slave Role).
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

#include "includes.h"

/*********************************************************************//**
 * @brief      SBL Initialize for master/slave
 * @param[in]  pConfig  SBL configuration.
 * @return     SBL_OK/SBL_INVALID_PARAM
 **********************************************************************/
SBL_RET_CODE SBL_Init(void)
{
    /*
     * Initialize SPI pin connect
     * P0.15 - SCK;
     * P0.16 - SSEL - used as GPIO
     * P0.17 - MISO
     * P0.18 - MOSI
     */
    // P0.15: FuncNum = 3, PinMode = 0, OD = 0
    LPC_PINCON->PINSEL0 |= 0xC0000000;          // set bit 30:31
    LPC_PINCON->PINMODE0 &= 0x3FFFFFFF;         //Clear bit 30:31 
    LPC_PINCON->PINMODE_OD0 &= 0xFFFF7FFF;      //Clear bit 15

    // P0.16 ~ P0.18: FuncNum = 3, PinMode = 0, OD = 0
    LPC_PINCON->PINSEL1 |= 0x3F;               // Set bit 0:5
    LPC_PINCON->PINMODE1 &= 0xFFFFFFC0;        // Clear bit 0:5
    LPC_PINCON->PINMODE_OD0 &= 0xFFF8FFFF;      // Clear bit 16:18


    // Initialize SPI peripheral with parameter given in structure above
    SPI_Init(LPC_SPI);

    return SBL_OK;
}

/*********************************************************************//**
 * @brief      De-Initialize a SBL device.
 * @param[in]  None
 * @return     SBL_OK
 **********************************************************************/
SBL_RET_CODE SBL_DeInit(void)
{
    SPI_DeInit(LPC_SPI);

    return SBL_OK;
}

/*********************************************************************//**
 * @brief      Read/Write data.
 * @param[in]  pBuffer  Store information about buffers for sending/receiving.
 * @return     SBL_OK/SBL_INVALID_PARAM/SBL_COMM_ERR
 **********************************************************************/
SBL_RET_CODE SBL_ReadWrite(PSBL_TRANSER_Type pBuffer)
{
    SPI_DATA_SETUP_Type xferConfig;

    if((pBuffer->TxLen == 0 && pBuffer->RxLen == 0))
    {
        return SBL_OK;
    }

    if((pBuffer->TxBuf == NULL && pBuffer->TxLen > 0) ||
        (pBuffer->RxBuf == NULL && pBuffer->RxLen > 0))
        return SBL_INVALID_PARAM;

    
    xferConfig.tx_data = pBuffer->TxBuf;
    xferConfig.rx_data = pBuffer->RxBuf;
    xferConfig.length = (pBuffer->TxLen > pBuffer->RxLen) ? pBuffer->TxLen: pBuffer->RxLen;

    
    if(SPI_ReadWrite(LPC_SPI, &xferConfig) != xferConfig.length)
    {
        return SBL_COMM_ERR;
    }

    return SBL_OK;
}



