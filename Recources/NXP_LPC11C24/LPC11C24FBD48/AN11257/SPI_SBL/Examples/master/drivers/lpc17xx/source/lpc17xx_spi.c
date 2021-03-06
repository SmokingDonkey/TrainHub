/**********************************************************************
* $Id$      lpc17xx_spi.c                2012-05-09
*//**
* @file     lpc17xx_spi.c
* @brief    Contains all functions support for SPI firmware library on LPC17xx
* @version  2.0
* @date     21. May. 2012
* @author   NXP MCU SW Application Team
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
/** @addtogroup SPI
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_spi.h"

#define CLKPWR_PCONP_PCSPI                  (1<<8)
#define SPI_CPHA_USED                         (SPI_CPHA_SECOND)
#define SPI_CPOL_USED                         (SPI_CPOL_LO)
#define SPI_CLK_RATE_USED                    (1000000)
#define SPI_DATA_ORDER_USED                    (SPI_DATA_MSB_FIRST)
#define SPI_DATA_BIT_USED                    (SPI_DATABIT_8)
#define SSP_DATA_WORD                       (0)
#define SPI_MODE_USED                        (SPI_MASTER_MODE)
#if TIMEOUT_ENABLE
#define SPI_RW_TIMEOUT                      (0x1000000)
#endif


/* Public Functions ----------------------------------------------------------- */
/** @addtogroup SPI_Public_Functions
 * @{
 */

/*********************************************************************//**
 * @brief        De-initializes the LPC_SPI peripheral registers to their
*                  default reset values.
 * @param[in]    None
 * @return       None
 **********************************************************************/
void SPI_DeInit()
{
    /* Set up clock and power for SPI module */
    LPC_SC->PCONP &= ~CLKPWR_PCONP_PCSPI;
}

/********************************************************************//**
 * @brief       Initializes the LPC_SPI peripheral according to the specified
 *               parameters in the UART_ConfigStruct.
 * @param[in]   None.
 * @return      None
 *********************************************************************/
void SPI_Init()
{
    uint32_t tmp;

    /* Set up clock and power for UART module */
    LPC_SC->PCONP |= CLKPWR_PCONP_PCSPI;

    // Configure SPI, interrupt is disable as default
    tmp = (SPI_CPHA_USED | SPI_CPOL_USED \
        | SPI_DATA_ORDER_USED | SPI_DATA_BIT_USED \
        | SPI_MODE_USED | SPI_SPCR_BIT_EN) & SPI_SPCR_BITMASK;
    // write back to SPI control register
    LPC_SPI->SPCR = tmp;

    // Set clock rate for SPI peripheral
    LPC_SPI->SPCCR = 0x0E;

    // If interrupt flag is set, Write '1' to Clear interrupt flag
    if (LPC_SPI->SPINT & SPI_SPINT_INTFLAG){
        LPC_SPI->SPINT = SPI_SPINT_INTFLAG;
    }
}

/*********************************************************************//**
 * @brief         SPI     Read write data function
 * @param[in]     dataCfg       Pointer to a SPI_DATA_SETUP_Type structure that
 *                              contains specified information about transmit
 *                    data configuration.
 * @return        Actual        Data length has been transferred in polling mode.
 *                              In interrupt mode, always return (0)
 *                              Return (-1) if error.
 * Note: This function can be used in both master and slave mode.
 ***********************************************************************/
int32_t SPI_ReadWrite ( SPI_DATA_SETUP_Type *dataCfg)
{
    uint8_t *rdata8;
    uint8_t *wdata8;
    uint16_t *rdata16;
    uint16_t *wdata16;
    uint32_t stat;
    uint32_t temp;
    uint8_t dataword;

    //read for empty buffer
    temp = LPC_SPI->SPDR;
    //dummy to clear status
    temp = LPC_SPI->SPSR;
    dataCfg->counter = 0;
    dataCfg->status = 0;

    dataword = SSP_DATA_WORD;

    if (dataword == 0){
        rdata8 = (uint8_t *)dataCfg->rx_data;
        wdata8 = (uint8_t *)dataCfg->tx_data;
    } else {
        rdata16 = (uint16_t *)dataCfg->rx_data;
        wdata16 = (uint16_t *)dataCfg->tx_data;
    }

        while(dataCfg->counter < dataCfg->length)
        {
            // Write data to buffer
            if(dataCfg->tx_data == NULL){
                if (dataword == 0){
                    LPC_SPI->SPDR = 0xFF;
                } else {
                    LPC_SPI->SPDR = 0xFFFF;
                }
            } else {
                if (dataword == 0){
                    LPC_SPI->SPDR = *wdata8;
                    wdata8++;
                } else {
                    LPC_SPI->SPDR = *wdata16;
                    wdata16++;
                }
            }
            // Wait for transfer complete
            while (!((stat = LPC_SPI->SPSR) & SPI_SPSR_SPIF));
            // Check for error
            if (stat & (SPI_SPSR_ABRT | SPI_SPSR_MODF | SPI_SPSR_ROVR | SPI_SPSR_WCOL)){
                // save status
                dataCfg->status = stat | SPI_STAT_ERROR;
                return (dataCfg->counter);
            }
            // Read data from SPI dat
            temp = LPC_SPI->SPDR & SPI_SPDR_BITMASK;

            // Store data to destination
            if (dataCfg->rx_data != NULL)
            {
                if (dataword == 0){
                    *(rdata8) = (uint8_t) temp;
                    rdata8++;
                } else {
                    *(rdata16) = (uint16_t) temp;
                    rdata16++;
                }
            }
            // Increase counter
            if (dataword == 0){
                dataCfg->counter++;
            } else {
                dataCfg->counter += 2;
            }
        }

        // Return length of actual data transferred
        // save status
        dataCfg->status = stat | SPI_STAT_DONE;
        return (dataCfg->counter);

}

/**
 * @}
 */


/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
