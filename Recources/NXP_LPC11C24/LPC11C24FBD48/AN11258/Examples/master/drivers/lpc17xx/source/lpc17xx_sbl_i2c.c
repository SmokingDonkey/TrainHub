/**********************************************************************
* $Id$        lpc17xx_sbl_spi.c            2012-05-04
*//**
* @file       lpc17xx_sbl_spi.c    
* @brief      Contain API for SBL transmission using SPI (Master Role).
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
#include "lpc17xx_i2c.h"

#if (SBL_CONNECTION == SBL_I2C)

/*********************************************************************//**
 * @brief        SBL Initialize for master/slave
 * @param[in]    None.
 * @return       SBL_OK/SBL_INVALID_PARAM
 **********************************************************************/
SBL_RET_CODE SBL_Init(void)
{

#if (I2C_DEV_ID == 0)
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

#elif (I2C_DEV_ID == 1)
    /*
     * Initialize SPI pin connect
     * P0.19 - SDA1
     * P0.20 - SCL1
     */
     // P0.19, P0.20: FuncNum = 2, OD = 1, Pull-up.
    LPC_PINCON->PINSEL1 = (LPC_PINCON->PINSEL1 & (~(0x0F<<6)))|(0x0A<<6);  
    LPC_PINCON->PINMODE1 &= ~(0x0F<<6);  
    LPC_PINCON->PINMODE_OD0 |= (0x03<<19);   
#if (I2C_CLK_RATE_USED > 400000) 
    #error Only I2C0 support Fast Mode Plus
#endif            
#else
    /*
     * Initialize SPI pin connect
     * P0.10 - SDA2
     * P0.11 - SCL2
     */
    // P0.10, P0.11: FuncNum = 2, OD = 1, Pull-up.
    LPC_PINCON->PINSEL0 = (LPC_PINCON->PINSEL0 & (~(0x0F<<20)))|(0x0A<<20);  
    LPC_PINCON->PINMODE0 &= ~(0x0F<<20);  
    LPC_PINCON->PINMODE_OD0 |= (0x03<<10);        
#if (I2C_CLK_RATE_USED > 400000) 
    #error Only I2C0 support Fast Mode Plus
#endif 
#endif
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


