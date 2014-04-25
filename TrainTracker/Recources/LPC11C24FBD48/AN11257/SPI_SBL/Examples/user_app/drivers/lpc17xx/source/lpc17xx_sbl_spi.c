/**********************************************************************
* $Id$        lpc17xx_sbl_spi.c            2012-05-04
*//**
* @file       lpc17xx_sbl_spi.c    
* @brief      Contain API for SBL transmission.
* @version    1.0
* @date       04. May. 2012
* @author     NXP MCU SW Application Team
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
#include "sbl.h"
#include "sbl_slave.h"
#include "sector0_program.h"
#include "includes.h"

/*********************************************************************//**
 * @brief        Check if it is able to send data.
 * @param[in]    None
 * @return       TRUE/FALSE
 **********************************************************************/
Bool SBL_TxReady(void)
{
    return TRUE;
}

/*********************************************************************//**
 * @brief        Check if there is data for receiving.
 * @param[in]    data 
 * @return       TRUE/FALSE
 **********************************************************************/
Bool SBL_RxReady(void)
{
    return (SPI_GetStatus(LPC_SPI) & SPI_STAT_SPIF) ? TRUE:FALSE;
}
/*********************************************************************//**
 * @brief        Send a byte.
 * @param[in]    data
 * @return       SBL_OK
 **********************************************************************/
void SBL_SendByte(uint8_t data)
{
    SPI_SendData(LPC_SPI, data );
}
/*********************************************************************//**
 * @brief        Start to receive.
 * @param[in]    None
 * @return       SBL_OK
 **********************************************************************/
void SBL_StartReceive(void)
{
    //read for empty buffer
    SPI_ReceiveData(LPC_SPI);

    SPI_SendData(LPC_SPI, 0xFF);
}
/*********************************************************************//**
 * @brief        Receive a byte.
 * @param[in]    None
 * @return       SBL_OK
 **********************************************************************/
uint8_t SBL_ReceiveByte(void)
{
    return SPI_ReceiveData(LPC_SPI);
}

/*********************************************************************//**
 * @brief        Read/Write data.
 * @param[in]    pBuffer  Store information about buffers for sending/receiving.
 * @return       SBL_OK/SBL_INVALID_PARAM/SBL_COMM_ERR
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



