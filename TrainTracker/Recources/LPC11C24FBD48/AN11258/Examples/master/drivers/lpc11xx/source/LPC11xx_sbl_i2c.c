/**********************************************************************
* $Id$         lpc11xx_sbl_i2c.c            2012-05-021
*//**
* @file        lpc11xx_sbl_i2c.c    
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

#include "sbl.h"
#include "lpc11xx_i2c.h"

#if (SBL_CONNECTION == SBL_I2C)

/*********************************************************************//**
 * @brief        SBL Initialize for master/slave
 * @param[in]    None.
 * @return       SBL_OK/SBL_INVALID_PARAM
 **********************************************************************/
SBL_RET_CODE SBL_Init(void)
{
    // Initialize Slave I2C peripheral
    I2C_Init();
    
    return SBL_OK;
}
#if 0
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
#endif
/*********************************************************************//**
 * @brief        Reset Communication
 * @param[in]    None.
 * @return       SBL_OK/SBL_INVALID_PARAM
 **********************************************************************/
SBL_RET_CODE SBL_Reset(void)
{
   I2C_Reset(); 
   return SBL_OK;
}
/*********************************************************************//**
 * @brief        Start to write a new frame.
 * @param[in]    None.
 * @return       None.
 **********************************************************************/
 void StartFrame(void)
{
    I2C_Start();
}
/*********************************************************************//**
 * @brief        End of a frame.
 * @param[in]    None.
 * @return       None.
 **********************************************************************/
 void EndFrame(void)
{
    I2C_Stop();
}

/*********************************************************************//**
 * @brief        Read/Write data.
 * @param[in]    pBuffer  Store information about buffers for sending/receiving.
 * @return       SBL_OK/SBL_INVALID_PARAM/SBL_COMM_ERR
 **********************************************************************/
SBL_RET_CODE SBL_ReadWrite(PSBL_TRANSER_Type pBuffer)
{
    I2C_M_SETUP_Type transferMCfg;

    if((pBuffer->TxLen == 0 && pBuffer->RxLen == 0))
    {
        return SBL_OK;
    }

    if((pBuffer->RxBuf == NULL && pBuffer->RxLen > 0))
        return SBL_INVALID_PARAM;

        
    /* Start I2C slave device first */
    transferMCfg.sl_addr7bit = I2C_SLV_ADDR;
    transferMCfg.tx_data = pBuffer->TxBuf;
    transferMCfg.tx_length = pBuffer->TxLen;
    transferMCfg.rx_data = pBuffer->RxBuf;
    transferMCfg.rx_length = pBuffer->RxLen;
    transferMCfg.retransmissions_max = 3;
        
    if(I2C_MasterTransferData(&transferMCfg) != SUCCESS)
    {
        return SBL_COMM_ERR;
    }
    
    return SBL_OK;
}
#endif /*(SBL_CONNECTION == SBL_I2C)*/


