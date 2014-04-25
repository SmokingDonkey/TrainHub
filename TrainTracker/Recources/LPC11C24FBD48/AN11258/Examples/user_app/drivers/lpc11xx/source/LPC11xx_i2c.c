/**********************************************************************
* $Id$         lpc11xx_i2c.c                2011-03-31
*//**
* @file        lpc11xx_i2c.c
* @brief       Contains all functions support for I2C firmware
*              library on LPC17xx
* @version     2.1
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
#include "includes.h"

#if (SBL_CONNECTION == SBL_I2C)
/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup I2C
 * @{
 */


/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */

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
    temp = (SystemCoreClock) / I2C_CLK_RATE_USED;
    I2C_DEV->SCLH = (uint32_t)(temp/2 );
    I2C_DEV->SCLL = (uint32_t)(temp - I2C_DEV->SCLH);
    /* Set I2C operation to default */
    I2C_DEV->CONCLR = 0xFF;
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
 * @brief        Enable or disable I2C peripheral's operation
 * @param[in]    NewState New State of I2C_DEV peripheral's operation
 * @return       none
 **********************************************************************/
void I2C_Cmd(FunctionalState NewState)
{

    if (NewState == ENABLE)
    {
        I2C_DEV->CONSET = I2C_CONSET_I2EN | I2C_CONSET_AA;
    }
    else
    {
        I2C_DEV->CONCLR = I2C_CONCLR_I2ENC;
    }
}
/*********************************************************************//**
 * @brief        Start to send/receive data.
 * @param[in]    None.
 * @return       None
 **********************************************************************/
void I2C_StartTransfer(void)
{
    /* Set AA bit to ACK command on I2C bus */
    if((I2C_DEV->CONSET & I2C_CONSET_AA) == 0)
        I2C_DEV->CONSET = I2C_CONSET_AA;
}
/*********************************************************************//**
 * @brief        Handle I2C Slave states.
 * @param[in]    CodeStatus    I2C state
 * @param[in]    TransferCfg   Pointer to a I2C_S_SETUP_Type structure that
 *                 contains specified information about the
 *                 configuration for master transfer.
 * @return         It can be
 *                - I2C_OK
 *                - I2C_BYTE_RECV
 *                - I2C_BYTE_SENT
 *                - I2C_SEND_END
 *                - I2C_RECV_END
 *                - I2C_ERR
 *                - I2C_NAK_RECV
 **********************************************************************/
int32_t I2C_SlaveHanleStates(uint32_t CodeStatus, I2C_S_SETUP_Type *TransferCfg)
{

    int32_t Ret = I2C_OK;
    uint8_t *txdat;
    uint8_t *rxdat;

    //get buffer to send/receive
    txdat = (uint8_t *) &TransferCfg->tx_data[TransferCfg->tx_count];
    rxdat = (uint8_t *) &TransferCfg->rx_data[TransferCfg->rx_count];
    
    switch (CodeStatus)
    {
        /* Reading phase -------------------------------------------------------- */
        /* Own SLA+R has been received, ACK has been returned */
        case I2C_I2STAT_S_RX_SLAW_ACK:

        /* General call address has been received, ACK has been returned */
        case I2C_I2STAT_S_RX_GENCALL_ACK:
            I2C_DEV->CONSET = I2C_CONSET_AA;
            I2C_DEV->CONCLR = I2C_CONCLR_SIC;
            Ret = I2C_BYTE_RECV;
            break;
        /* Arbitration has been lost in Slave Address + R/W bit as bus Master. General Call has
            been received and ACK has been returned.*/
        case I2C_I2STAT_S_RX_ARB_LOST_M_GENCALL:
            I2C_DEV->CONCLR = I2C_CONCLR_SIC;
            I2C_DEV->CONSET = I2C_CONSET_AA|I2C_CONSET_STA;
            break;
        /* Previously addressed with own SLA;
         * DATA byte has been received;
         * ACK has been returned */
        case I2C_I2STAT_S_RX_ARB_LOST_M_SLA:
        case I2C_I2STAT_S_RX_PRE_SLA_DAT_ACK:

            /*
             * All data bytes that over-flow the specified receive
             * data length, just ignore them.
             */
            if ((TransferCfg->rx_count < TransferCfg->rx_length) && (TransferCfg->rx_data != NULL))
            {
                *rxdat++ = (uint8_t)I2C_DEV->DAT;

                TransferCfg->rx_count++;

                Ret = I2C_BYTE_RECV;
            }
            I2C_DEV->CONSET = I2C_CONSET_AA;
            I2C_DEV->CONCLR = I2C_CONCLR_SIC;

            if(TransferCfg->rx_count >= TransferCfg->rx_length) {
                Ret = I2C_RECV_END;
            }
            break;
        /* DATA has been received, Only the first data byte will be received with ACK. Additional
                data will be received with NOT ACK. */
        case I2C_I2STAT_S_RX_PRE_GENCALL_DAT_ACK:
            if ((TransferCfg->rx_count < TransferCfg->rx_length) && (TransferCfg->rx_data != NULL))
            {
                *rxdat++ = (uint8_t)I2C_DEV->DAT;

                TransferCfg->rx_count++;

                Ret = I2C_BYTE_RECV;
            }
            I2C_DEV->CONCLR = I2C_CONCLR_AAC|I2C_CONCLR_SIC;
            break;
        /* Previously addressed with own SLA;
         * DATA byte has been received;
         * NOT ACK has been returned */
        case I2C_I2STAT_S_RX_PRE_SLA_DAT_NACK:    
        /* DATA has been received, NOT ACK has been returned */
        case I2C_I2STAT_S_RX_PRE_GENCALL_DAT_NACK:
            if ((TransferCfg->rx_count < TransferCfg->rx_length) && (TransferCfg->rx_data != NULL))
            {
                *rxdat++ = (uint8_t)I2C_DEV->DAT;

                TransferCfg->rx_count++;
            }
            I2C_DEV->CONSET = I2C_CONSET_AA;
            I2C_DEV->CONCLR = I2C_CONCLR_SIC;
            Ret = I2C_RECV_END;
            break;

        /* Writing phase -------------------------------------------------------- */
        /* Own SLA+R has been received, ACK has been returned */
        case I2C_I2STAT_S_TX_SLAR_ACK:

        /* Data has been transmitted, ACK has been received */
        case I2C_I2STAT_S_TX_DAT_ACK:
            /*
             * All data bytes that over-flow the specified receive
             * data length, just ignore them.
             */
            if ((TransferCfg->tx_count < TransferCfg->tx_length) && (TransferCfg->tx_data != NULL))
            {
                I2C_DEV->DAT = *txdat++;

                TransferCfg->tx_count++;

                Ret = I2C_BYTE_SENT;
            }
            I2C_DEV->CONSET = I2C_CONSET_AA;
            I2C_DEV->CONCLR = I2C_CONCLR_SIC;
            break;
        /* Arbitration lost in Slave Address and R/W bit as bus Master. Own Slave Address + Read
                has been received, ACK has been returned. */
        case I2C_I2STAT_S_TX_ARB_LOST_M_SLA:
            if ((TransferCfg->tx_count < TransferCfg->tx_length) && (TransferCfg->tx_data != NULL))
            {
                I2C_DEV->DAT = *txdat++;

                TransferCfg->tx_count++;

                Ret = I2C_BYTE_SENT;
            }
            I2C_DEV->CONCLR = I2C_CONCLR_SIC;
            I2C_DEV->CONSET = I2C_CONSET_AA|I2C_CONSET_STA;
            break;
            
        case I2C_I2STAT_S_TX_LAST_DAT_ACK:
        /* Data has been transmitted, NACK has been received,
         * that means there's no more data to send, exit now */
        /*
         * Note: Don't wait for stop event since in slave transmit mode,
         * since there no proof lets us know when a stop signal has been received
         * on slave side.
         */
        case I2C_I2STAT_S_TX_DAT_NACK:
            I2C_DEV->CONCLR = I2C_CONCLR_SIC;
            Ret = I2C_SEND_END;
            break;

        /*
         * Note that: Return code only let us know a stop condition mixed
         * with a repeat start condition in the same code value.
         * So we should provide a time-out. In case this is really a stop
         * condition, this will return back after time out condition. Otherwise,
         * next session that is slave receive data will be completed.
         */

        /* A Stop or a repeat start condition */
        case I2C_I2STAT_S_RX_STA_STO_SLVREC_SLVTRX:
            I2C_DEV->CONSET = I2C_CONSET_AA;
            I2C_DEV->CONCLR = I2C_CONCLR_SIC;
            Ret = I2C_STA_STO_RECV;
            break;
        case I2C_I2STAT_BUS_ERROR:
        /* No status information */
        case I2C_I2STAT_NO_INF:
        /* Other status must be captured */
        default:
            I2C_DEV->CONSET = I2C_CONSET_AA;
            I2C_DEV->CONCLR = I2C_CONCLR_SIC;
            Ret = I2C_OK;
            break;
        
    }

    return Ret;
}

/*********************************************************************//**
 * @brief         Receive and Transmit data in slave mode
  * @param[in]    TransferCfg        Pointer to a I2C_S_SETUP_Type structure that
 *                                 contains specified information about the
 *                                 configuration for master transfer.
 * @return        SUCCESS or ERROR
 *
 * Note:
 * The mode of slave's operation depends on the command sent from master on
 * the I2C bus. If the master send a SLA+W command, this sub-routine will
 * use receive data length and receive data pointer. If the master send a SLA+R
 * command, this sub-routine will use transmit data length and transmit data
 * pointer.
 * If the master issue an repeat start command or a stop command, the slave will
 * enable an time out condition, during time out condition, if there's no activity
 * on I2C bus, the slave will exit, otherwise (i.e. the master send a SLA+R/W),
 * the slave then switch to relevant operation mode. The time out should be used
 * because the return status code can not show difference from stop and repeat
 * start command in slave operation.
 * In case of the expected data length from master is greater than data length
 * that slave can support:
 * - In case of reading operation (from master): slave will return I2C_I2DAT_IDLE_CHAR
 * value.
 * - In case of writing operation (from master): slave will ignore remain data from master.
 **********************************************************************/
Status I2C_SlaveTransferData(I2C_S_SETUP_Type *TransferCfg)
{
    int32_t   Ret = I2C_OK;
    uint8_t   StartTransfer = FALSE;
    
    uint32_t CodeStatus;

    // Reset I2C setup value to default state
    TransferCfg->tx_count = 0;
    TransferCfg->rx_count = 0;
    TransferCfg->status = 0;

    // Polling option
    {    
        while (1)
        {
            /* Check SI flag ready */
            if (I2C_DEV->CONSET & I2C_CONSET_SI)
            {

                CodeStatus = (I2C_DEV->STAT & I2C_STAT_CODE_BITMASK);

                Ret = I2C_SlaveHanleStates(CodeStatus, TransferCfg);
                if(I2C_CheckError(Ret))
                {
                    goto s_error;
                }
                else if((Ret & (I2C_RECV_END|I2C_SEND_END)))
                {
                    if((TransferCfg->tx_count >= TransferCfg->tx_length) &&
                        (TransferCfg->rx_count >= TransferCfg->rx_length))
                            goto s_end_stage;
                }
                else if((Ret == I2C_STA_STO_RECV) && StartTransfer)
                {
                    if((TransferCfg->tx_count >= TransferCfg->tx_length) &&
                        (TransferCfg->rx_count >= TransferCfg->rx_length))
                            goto s_end_stage;
                }
                else if((Ret & (I2C_BYTE_RECV|I2C_BYTE_SENT)))
                {
                    StartTransfer = TRUE;
                }

               
            }
            else
            {
                if(!(I2C_DEV->CONSET & I2C_CONSET_AA))
                {
                   I2C_DEV->CONSET = I2C_CONSET_AA;
                }
            }
        }

s_end_stage:
        ///* Clear AA bit to disable ACK on I2C bus */
        I2C_DEV->CONCLR = I2C_CONCLR_AAC;

        // Check if there's no error during operation
        // Update status
        TransferCfg->status = CodeStatus | I2C_SETUP_STATUS_DONE;
        return SUCCESS;

s_error:
       // /* Clear AA bit to disable ACK on I2C bus */
        I2C_DEV->CONCLR = I2C_CONCLR_AAC;

        // Update status
        TransferCfg->status = CodeStatus;
        return ERROR;
    }

}

/*********************************************************************//**
 * @brief        Set Own slave address in I2C peripheral corresponding to
 *                 parameter specified in OwnSlaveAddrConfigStruct.
 * @param[in]    None
 * @return       None
 **********************************************************************/
void I2C_SetOwnSlaveAddr(void)
{
    uint32_t tmp;

    tmp = (((uint32_t)(I2C_SLV_ADDR << 1)) )& I2C_I2ADR_BITMASK;
    I2C_DEV->ADR0 = tmp;
    I2C_DEV->MASK0 = I2C_I2MASK_MASK(0xFF);
}
/**
 * @}
 */

/**
 * @}
 */
#endif /*(SBL_CONNECTION == SBL_I2C)*/
/* --------------------------------- End Of File ------------------------------ */
