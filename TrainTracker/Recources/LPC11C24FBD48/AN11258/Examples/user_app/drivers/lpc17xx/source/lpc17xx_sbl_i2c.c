/**********************************************************************
* $Id$         lpc17xx_sbl_i2c.c            2012-05-21
*//**
* @file        lpc17xx_sbl_i2c.c    
* @brief       Contain API for SBL transmission using I2C (Slave Role).
* @version     1.0
* @date        21. May. 2012
* @author      NXP MCU SW Application Team
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

#if (SBL_CONNECTION == SBL_I2C)
/*********************************************************************//**
 * @brief        Check if it is able to send data.
 * @param[in]    None
 * @return       TRUE/FALSE
 **********************************************************************/
Bool SBL_TxReady(void)
{
    uint32_t CodeStatus;
    /* Set AA bit to ACK command on I2C bus */
    if((I2C_DEV->I2CONSET & I2C_I2CONSET_AA) == 0)
        I2C_DEV->I2CONSET = I2C_I2CONSET_AA;
    
    if((I2C_DEV->I2CONSET & I2C_I2CONSET_SI) == 0)
        return FALSE;

    CodeStatus = (I2C_DEV->I2STAT & I2C_STAT_CODE_BITMASK);

    if((CodeStatus == I2C_I2STAT_S_TX_SLAR_ACK)||
       (CodeStatus == I2C_I2STAT_S_TX_DAT_ACK) ||
       (CodeStatus == I2C_I2STAT_S_TX_DAT_NACK))
       return TRUE;

    if(CodeStatus == I2C_I2STAT_BUS_ERROR)
    {
        I2C_Cmd(DISABLE);
        I2C_Cmd(ENABLE);
    }
    return FALSE;
}

/*********************************************************************//**
 * @brief        Check if there is data for receiving.
 * @param[in]    data 
 * @return       TRUE/FALSE
 **********************************************************************/
Bool SBL_RxReady(void)
{
    uint32_t CodeStatus;

   if((I2C_DEV->I2CONSET & I2C_I2CONSET_SI) == 0)
       return FALSE;

   CodeStatus = (I2C_DEV->I2STAT & I2C_STAT_CODE_BITMASK);

   if((CodeStatus == I2C_I2STAT_S_RX_SLAW_ACK)||
       (CodeStatus == I2C_I2STAT_S_RX_GENCALL_ACK) ||
       (CodeStatus == I2C_I2STAT_S_RX_PRE_SLA_DAT_ACK)||
       (CodeStatus == I2C_I2STAT_S_RX_PRE_SLA_DAT_NACK)||
       (CodeStatus == I2C_I2STAT_S_RX_PRE_GENCALL_DAT_ACK)||
       (CodeStatus == I2C_I2STAT_S_RX_PRE_GENCALL_DAT_NACK)||
       (CodeStatus == I2C_I2STAT_S_RX_STA_STO_SLVREC_SLVTRX))
       return TRUE;

   return FALSE;
}
/*********************************************************************//**
 * @brief        Send a byte.
 * @param[in]    data
 * @return       SBL_OK
 **********************************************************************/
void SBL_SendByte(uint8_t data)
{
    I2C_S_SETUP_Type transferSCfg;
       
    /* Start I2C slave device first */
    transferSCfg.tx_data = &data;
    transferSCfg.tx_length = 1;
    transferSCfg.rx_data = NULL;
    transferSCfg.rx_length = 0;
    I2C_SlaveTransferData(&transferSCfg);
}
/*********************************************************************//**
 * @brief        Start to receive.
 * @param[in]    None
 * @return       SBL_OK
 **********************************************************************/
void SBL_StartReceive(void)
{
     /* Set AA bit to ACK command on I2C bus */
    I2C_DEV->I2CONSET = I2C_I2CONSET_AA;
}
/*********************************************************************//**
 * @brief        Receive a byte.
 * @param[in]    None
 * @return       SBL_OK
 **********************************************************************/
uint8_t SBL_ReceiveByte(void)
{
    I2C_S_SETUP_Type transferSCfg;
       uint8_t data = 0xFF;

    /* Start I2C slave device first */
    transferSCfg.tx_data = NULL;
    transferSCfg.tx_length = 0;
    transferSCfg.rx_data = &data;
    transferSCfg.rx_length = 1;
    I2C_SlaveTransferData(&transferSCfg);

    return data;
}

/*********************************************************************//**
 * @brief        Read/Write data.
 * @param[in]    pBuffer  Store information about buffers for sending/receiving.
 * @return       SBL_OK/SBL_INVALID_PARAM/SBL_COMM_ERR
 **********************************************************************/
SBL_RET_CODE SBL_ReadWrite(PSBL_TRANSER_Type pBuffer)
{
    I2C_S_SETUP_Type transferSCfg;

    if((pBuffer->TxLen == 0 && pBuffer->RxLen == 0))
    {
        return SBL_OK;
    }

    if((pBuffer->TxBuf == NULL && pBuffer->TxLen > 0) ||
        (pBuffer->RxBuf == NULL && pBuffer->RxLen > 0))
        return SBL_INVALID_PARAM;
        
    /* Start I2C slave device first */
    transferSCfg.tx_data = pBuffer->TxBuf;
    transferSCfg.tx_length = pBuffer->TxLen;
    transferSCfg.rx_data = pBuffer->RxBuf;
    transferSCfg.rx_length = pBuffer->RxLen;
    I2C_StartTransfer();
    
    if(I2C_SlaveTransferData(&transferSCfg) != SUCCESS)
    {
        return SBL_COMM_ERR;
    }
    return SBL_OK;
}
#endif /*(SBL_CONNECTION == SBL_I2C)*/


