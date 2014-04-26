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
 * @brief        SBL Initialize for master/slave
 * @param[in]    pConfig  SBL configuration.
 * @return       SBL_OK/SBL_INVALID_PARAM
 **********************************************************************/
SBL_RET_CODE SBL_Init(void)
{
    /*
     * Initialize SPI pin connect
     * P0.27 - SDA0
     * P0.28 - SCL0
     */
    // P0.27, P0.28: FuncNum = 1, OD = 1.
    LPC_PINCON->PINSEL1 = (LPC_PINCON->PINSEL1 & (~(0x0F<<22)))|(0x05<<22);   
           
#if (I2C_CLK_RATE_USED > 400000) 
    LPC_PINCON->I2CPADCFG |= 0x05; // Set SDADRV0 and SCLDRV0 bits . 
#endif

    // Initialize Slave I2C peripheral
    I2C_Init();

    /* Set  Own slave address for I2C device */
    I2C_SetOwnSlaveAddr();

    /* Enable Slave I2C operation */
    I2C_Cmd(ENABLE);

    return SBL_OK;
}

/*********************************************************************//**
 * @brief        De-Initialize a SBL device.
 * @param[in]    None
 * @return       SBL_OK
 **********************************************************************/
SBL_RET_CODE SBL_DeInit(void)
{
    I2C_DeInit();

    return SBL_OK;
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


