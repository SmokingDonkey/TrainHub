/**********************************************************************
* $Id$         lpc11xx_i2c.c                2011-03-31
*//**
* @file        lpc11xx_i2c.c
* @brief       Contains all functions support for I2C firmware
*              library on LPC17xx
 * @version    1.0
 * @date       9. April. 2009
* @author      NXP MCU SW Application Team
*
* Copyright(C) 2010, NXP Semiconductor
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

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup I2C
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "sbl.h"
#include "lpc11xx_i2c.h"
#include "lpc11xx_clkpwr.h"
#include "lpc11xx_pinsel.h"


#if (SBL_CONNECTION == SBL_I2C)
/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */

/* Private Functions ---------------------------------------------------------- */
/* I2C send byte subroutine */
static void I2C_SendByte (uint8_t databyte);

/* I2C get byte subroutine */
static void I2C_GetByte (uint8_t *retdat, Bool ack);


/*--------------------------------------------------------------------------------*/
/********************************************************************//**
 * @brief        Send a byte
 * @param[in]    databyte: number of byte
 * @return       None
 *********************************************************************/
static void I2C_SendByte ( uint8_t databyte)
{
    I2C_DEV->DAT = databyte & I2C_DAT_BITMASK;

    I2C_DEV->CONSET = I2C_CONSET_AA;
    I2C_DEV->CONCLR = I2C_CONCLR_SIC;
}

/********************************************************************//**
 * @brief        Get a byte
 * @param[out]   retdat    pointer to return data
 * @param[in]    ack        assert acknowledge or not, should be: TRUE/FALSE
 * @return       None
 *********************************************************************/
static void I2C_GetByte (uint8_t *retdat, Bool ack)
{
    *retdat = (uint8_t) (I2C_DEV->DAT & I2C_DAT_BITMASK);
    
    if (ack)
    {
        I2C_DEV->CONSET = I2C_CONSET_AA;
        I2C_DEV->CONCLR = I2C_CONCLR_SIC;
    }
    else
    {
        I2C_DEV->CONCLR = I2C_CONCLR_AAC|I2C_CONCLR_SIC;
     }

     
}
/********************************************************************//**
 * @brief       Wait for interupt
 * @param[in]    timeout  timeout value
 * @return       None
 *********************************************************************/
static __INLINE void I2C_WaitInt(uint32_t timeout)
{
    uint32_t tm = timeout;
    // Wait for sending ends                
    while ((!(I2C_DEV->CONSET & I2C_CONSET_SI)) && tm )
    {
        tm--;
    }
}
/* End of Private Functions --------------------------------------------------- */


/* Public Functions ----------------------------------------------------------- */
/** @addtogroup I2C_Public_Functions
 * @{
 */

/********************************************************************//**
 * @brief        Initializes the I2C_DEV peripheral with specified parameter.
 * @param[in]    None
 * @return       None
 *********************************************************************/
void I2C_Init(void)
{  
    uint32_t temp;

    /* It seems to be bit0 is for I2C, different from
    UM. To be retested along with SSP reset. SSP and I2C
    reset are overlapped, a known bug, for now, both SSP
    and I2C use bit 0 for reset enable. Once the problem
    is fixed, change to "#if 1". */
    #if 1
    LPC_SYSCON->PRESETCTRL |= (0x1<<1);
    #else
    LPC_SYSCON->PRESETCTRL |= (0x1<<0);
    #endif
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<5);
    LPC_IOCON->PIO0_4 &= ~0x1F;    /*  I2C I/O config */
    LPC_IOCON->PIO0_4 |= 0x01;        /* I2C SCL */
    LPC_IOCON->PIO0_5 &= ~0x1F;
    LPC_IOCON->PIO0_5 |= 0x01;        /* I2C SDA */
#if (I2C_CLK_RATE_USED > 400000)
    LPC_IOCON->PIO0_4 |= (0x02<<8);        /* Fast Mode Plus I2C */
    LPC_IOCON->PIO0_5 |= (0x02<<8);        /* Fast Mode Plus I2C */
#endif    
    /* Set clock rate */
    temp = SystemCoreClock  / I2C_CLK_RATE_USED;
    I2C_DEV->SCLH = (uint32_t)(temp/2 );
    I2C_DEV->SCLL = (uint32_t)(temp - I2C_DEV->SCLH);

    I2C_Reset();
}

/*********************************************************************//**
 * @brief        De-initializes the I2C peripheral registers to their
 *                  default reset values.
 * @param[in]    None
 * @return       None
 **********************************************************************/
void I2C_DeInit(void)
{
    /* Disable I2C control */
    I2C_DEV->CONCLR = I2C_CONCLR_I2ENC;
    /* Disable power for I2C0 module */
     LPC_SYSCON->SYSAHBCLKCTRL  =  LPC_SYSCON->SYSAHBCLKCTRL & (~(1<<5));
}
/*********************************************************************//**
 * @brief       Reset I2C Block
* @param[in]    None
 * @return      None
 **********************************************************************/
__INLINE void I2C_Reset(void)
{
    /* Disable I2C control */
    I2C_DEV->CONCLR = 0xFF;

    /* Enable Slave I2C operation */
    I2C_DEV->CONSET = I2C_CONSET_I2EN;
}
/********************************************************************//**
 * @brief        Generate a start condition on I2C bus (in master mode only)
 * @param[in]    None
 * @return       None
 *********************************************************************/
void I2C_Start (void)
{    
    volatile uint32_t timeout = 0;

restart:
    timeout = 0;
    // Enter to Master Mode
    I2C_DEV->CONCLR = I2C_CONCLR_SIC;
    I2C_DEV->CONSET = I2C_CONSET_STA;

    // Wait for complete
    while(1)
    {
        if((I2C_DEV->STAT == I2C_STAT_M_TX_START) ||
            (I2C_DEV->STAT == I2C_STAT_M_TX_RESTART))
            break;
       
        timeout++;
        if(timeout > 0x1000)
        {
            I2C_Reset();
            goto restart;
        }    
    }
}

/********************************************************************//**
 * @brief        Generate a stop condition on I2C bus (in master mode only)
 * @param[in]    None
 * @return       None
 *********************************************************************/
void I2C_Stop (void)
{
	I2C_DEV->CONCLR = I2C_CONCLR_SIC;
    I2C_DEV->CONSET = I2C_CONSET_STO;
    while(I2C_DEV->CONSET & I2C_CONSET_STO)
    {
        if(I2C_DEV->CONSET & I2C_CONSET_SI)
            I2C_DEV->CONCLR = I2C_CONCLR_SIC;
    }
}
/*********************************************************************//**
 * @brief        Handle I2C Master states.
 * @param[in]    CodeStatus    I2C state
 * @param[in]    TransferCfg   Pointer to a I2C_S_SETUP_Type structure that
 *                                 contains specified information about the
 *                                 configuration for master transfer.
 * @return       It can be
 *                - I2C_OK
 *                - I2C_BYTE_RECV
 *                - I2C_BYTE_SENT
 *                - I2C_SEND_END
 *                - I2C_RECV_END
 *                - I2C_ERR
 *                - I2C_NAK_RECV
 **********************************************************************/
int32_t I2C_MasterHanleStates(uint32_t PreStatus,uint32_t CodeStatus, I2C_M_SETUP_Type *TransferCfg)
{
    uint8_t tmp;
    switch(CodeStatus)
    {
        case I2C_STAT_M_TX_START:
        case I2C_STAT_M_TX_RESTART:
        //case I2C_STAT_M_RX_START:
        //case I2C_STAT_M_RX_RESTART
            // Send data first
            if(TransferCfg->tx_count < TransferCfg->tx_length)
            {
                /* Send slave address + WR direction bit = 0 ----------------------------------- */
                I2C_SendByte( (TransferCfg->sl_addr7bit << 1));
            }
            else 
            {
                /* Send slave address + RD direction bit = 1 ----------------------------------- */
                I2C_SendByte(((TransferCfg->sl_addr7bit << 1) | 0x01));
            }
            // Clear STA bit
            I2C_DEV->CONCLR = I2C_CONCLR_STAC;
            return I2C_BYTE_SENT;
        case I2C_STAT_M_TX_SLAW_ACK:
        case I2C_STAT_M_TX_DAT_ACK:
            
            if(TransferCfg->tx_count < TransferCfg->tx_length)
            {
                I2C_SendByte( TransferCfg->tx_data[TransferCfg->tx_count++]);
                return I2C_BYTE_SENT;
            }
            return I2C_SEND_END;
        case I2C_STAT_M_TX_DAT_NACK:
            I2C_DEV->CONCLR = I2C_CONCLR_SIC;
            if(TransferCfg->tx_count < TransferCfg->tx_length)
            {
                I2C_Start();
                return I2C_OK;
            }
            else
                return I2C_SEND_END;
            return I2C_SEND_END;
        case I2C_STAT_M_RX_SLAR_ACK:
            I2C_DEV->CONSET = I2C_CONSET_AA;
            I2C_DEV->CONCLR = I2C_CONCLR_SIC;
            return I2C_BYTE_RECV;
        case I2C_STAT_M_RX_DAT_ACK:
        case I2C_STAT_M_RX_DAT_NACK:
            if (TransferCfg->rx_length)
            {
                if(TransferCfg->rx_count >= TransferCfg->rx_length)
                    return I2C_RECV_END;
                if((TransferCfg->rx_length == 1)||(TransferCfg->rx_count > (TransferCfg->rx_length - 2)) )
                {
                    I2C_GetByte( &tmp, FALSE);    // the next byte is the last byte, send NACK
                }
                else
                {
                    I2C_GetByte( &tmp, TRUE);

                }
                TransferCfg->rx_data[TransferCfg->rx_count++] = tmp;
                if(TransferCfg->rx_count >= TransferCfg->rx_length)
                    return I2C_RECV_END;
            }
            else
            {
                if(TransferCfg->tx_count < TransferCfg->tx_length)
                    I2C_Start();
                else
                    return I2C_SEND_END;
            }
            return I2C_BYTE_RECV;
        case I2C_STAT_M_RX_SLAR_NACK:
        case I2C_STAT_M_TX_SLAW_NACK:
            I2C_DEV->CONCLR = I2C_CONCLR_SIC;
            return I2C_ERR;
        case I2C_STAT_M_TX_ARB_LOST:
            if(TransferCfg->tx_count < TransferCfg->tx_length)
                I2C_Start();
            else
                return I2C_SEND_END;
            return I2C_OK;
        //case I2C_I2STAT_M_TX_ARB_LOST:
        case I2C_STAT_S_RX_ARB_LOST_M_SLA:
        case I2C_STAT_S_RX_ARB_LOST_M_GENCALL:
            I2C_Stop(); 
            I2C_Start();
            return I2C_BYTE_RECV;
        case I2C_STAT_BUS_ERROR:
            I2C_Reset();
            if((TransferCfg->tx_count <= TransferCfg->tx_length)||(TransferCfg->rx_count < TransferCfg->rx_length))
            {
                I2C_Start();
                if((PreStatus == I2C_BYTE_SENT)&& TransferCfg->tx_count)     // sending failed
                {
                    TransferCfg->tx_count--;
                }
            }
            return I2C_OK; 
        /* No status information */
        case I2C_STAT_NO_INF:
        default:
              return I2C_OK;
    }

}

/*********************************************************************//**
 * @brief         Transmit and Receive data in master mode
 * @param[in]    TransferCfg        Pointer to a I2C_M_SETUP_Type structure that
 *                                 contains specified information about the
 *                                 configuration for master transfer.
 * @return         SUCCESS or ERROR
 *
 * Note:
 * - In case of using I2C to transmit data only, either transmit length set to 0
 * or transmit data pointer set to NULL.
 * - In case of using I2C to receive data only, either receive length set to 0
 * or receive data pointer set to NULL.
 * - In case of using I2C to transmit followed by receive data, transmit length,
 * transmit data pointer, receive length and receive data pointer should be set
 * corresponding.
 **********************************************************************/
Status I2C_MasterTransferData(I2C_M_SETUP_Type *TransferCfg)
{
    int32_t Ret = I2C_OK;
    const uint32_t timeout_max = 0x1000;
    uint32_t timeout = 0x100000;

    /* First Start condition -------------------------------------------------------------- */
    TransferCfg->retransmissions_count = 0;
    // Reset I2C setup value to default state
    TransferCfg->tx_count = 0;
    TransferCfg->rx_count = 0;
    while(1)    // send data first and then receive data from Slave.
    {
        Ret = I2C_MasterHanleStates(Ret, I2C_DEV->STAT & I2C_STAT_CODE_BITMASK, TransferCfg);
        
        
        if( Ret & (I2C_BYTE_SENT|I2C_BYTE_RECV))
        {
            I2C_WaitInt(timeout_max);
            timeout = timeout_max;
            if(!(I2C_DEV->CONSET & I2C_CONSET_SI)) 
              return ERROR;  
        }
        else if ((Ret & I2C_SEND_END) || (Ret & I2C_RECV_END))// If no need to wait for data from Slave// If no need to wait for data from Slave
        {
            if((TransferCfg->tx_count >= TransferCfg->tx_length) &&
                (TransferCfg->rx_count >= TransferCfg->rx_length))
                return SUCCESS;
        }
        else if (I2C_CheckError(Ret))
        {           
            if(I2C_CheckError(Ret) || (timeout == 0))
            {
                if (++TransferCfg->retransmissions_count > TransferCfg->retransmissions_max){
                        return ERROR;
                } 
                I2C_Start();
                timeout = timeout_max;
            }
        }
    }

}

/**
 * @}
 */

#endif /*(SBL_CONNECTION == SBL_I2C) */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
