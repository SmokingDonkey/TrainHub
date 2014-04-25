/**********************************************************************
* $Id$		lpc17xx_spi.c				2010-05-21
*//**
* @file		lpc17xx_spi.c
* @brief	Contains all functions support for SPI firmware library on LPC17xx
* @version	2.0
* @date		21. May. 2010
* @author	NXP MCU SW Application Team
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
#include "includes.h"
/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */

#define CHECK_PARAM(x)        
#define CLKPWR_PCONP_PCSPI       (1<<8)
#define SPI_CPHA_USED                         (SPI_CPHA_SECOND)
#define SPI_CPOL_USED                         (SPI_CPOL_LO)
#define SPI_CLK_RATE_USED                 (1000000)
#define SPI_DATA_ORDER_USED                  (SPI_DATA_MSB_FIRST)
#define SPI_DATA_BIT_USED                  (SPI_DATABIT_8)
#define SPI_MODE_USED                    (SPI_SLAVE_MODE)
#if TIMEOUT_ENABLE
#define SPI_RW_TIMEOUT                  (0x1000000)
#endif

/* Public Functions ----------------------------------------------------------- */
/** @addtogroup SPI_Public_Functions
 * @{
 */


/*********************************************************************//**
 * @brief        De-initializes the SPIx peripheral registers to their
*                  default reset values.
 * @param[in]    SPIx	SPI peripheral selected, should be LPC_SPI
 * @return 	 None
 **********************************************************************/
void SPI_DeInit(LPC_SPI_TypeDef *SPIx)
{
    CHECK_PARAM(PARAM_SPIx(SPIx));

    if (SPIx == LPC_SPI){
        /* Set up clock and power for SPI module */
        LPC_SC->PCONP &= ~CLKPWR_PCONP_PCSPI;
    }
}

/********************************************************************//**
 * @brief        Initializes the SPIx peripheral according to the specified
*                parameters in the UART_ConfigStruct.
 * @param[in]    SPIx	SPI peripheral selected, should be LPC_SPI
 * @return 	 None
 *********************************************************************/
void SPI_Init(LPC_SPI_TypeDef *SPIx)
{
    uint32_t tmp;

    CHECK_PARAM(PARAM_SPIx(SPIx));

    if(SPIx == LPC_SPI){
        /* Set up clock and power for UART module */
        LPC_SC->PCONP |= CLKPWR_PCONP_PCSPI;
    } else {
        return;
    }

    // Configure SPI, interrupt is disable as default
    tmp = (SPI_CPHA_USED | SPI_CPOL_USED \
        | SPI_DATA_ORDER_USED | SPI_DATA_BIT_USED \
        | SPI_MODE_USED | SPI_SPCR_BIT_EN) & SPI_SPCR_BITMASK;
    // write back to SPI control register
    SPIx->SPCR = tmp;

    // Set clock rate for SPI peripheral
    SPIx->SPCCR = 0x0E;

    // If interrupt flag is set, Write '1' to Clear interrupt flag
    if (SPIx->SPINT & SPI_SPINT_INTFLAG){
        SPIx->SPINT = SPI_SPINT_INTFLAG;
    }
}

/*********************************************************************//**
 * @brief        Transmit a single data through SPIx peripheral
 * @param[in]    SPIx	SPI peripheral selected, should be LPC_SPI
 * @param[in]    Data	Data to transmit (must be 16 or 8-bit long,
 * 						this depend on SPI data bit number configured)
 * @return 	 None
 **********************************************************************/
void SPI_SendData(LPC_SPI_TypeDef* SPIx, uint16_t Data)
{
    CHECK_PARAM(PARAM_SPIx(SPIx));

    SPIx->SPDR = Data & SPI_SPDR_BITMASK;
}



/*********************************************************************//**
 * @brief        Receive a single data from SPIx peripheral
 * @param[in]    SPIx	SPI peripheral selected, should be LPC_SPI
 * @return 	 Data received (16-bit long)
 **********************************************************************/
uint16_t SPI_ReceiveData(LPC_SPI_TypeDef* SPIx)
{
    CHECK_PARAM(PARAM_SPIx(SPIx));

    return ((uint16_t) (SPIx->SPDR & SPI_SPDR_BITMASK));
}

/*********************************************************************//**
 * @brief 	 SPI 	Read write data function
 * @param[in]    SPIx 	Pointer to SPI peripheral, should be LPC_SPI
 * @param[in]    dataCfg	Pointer to a SPI_DATA_SETUP_Type structure that
 * 						contains specified information about transmit
 * 						data configuration.
 * @return 	 Actual Data length has been transferred in polling mode.
 * 			In interrupt mode, always return (0)
 * 			Return (-1) if error.
 * Note: This function can be used in both master and slave mode.
 ***********************************************************************/
int32_t SPI_ReadWrite (LPC_SPI_TypeDef *SPIx, SPI_DATA_SETUP_Type *dataCfg)
{
    uint8_t *rdata8;
    uint8_t *wdata8;
    uint16_t *rdata16;
    uint16_t *wdata16;
    uint32_t stat;
    uint32_t temp;
    uint8_t dataword;
#if TIMEOUT_ENABLE
    uint32_t timeout = 0;
#endif

    //read for empty buffer
    temp = SPIx->SPDR;
    //dummy to clear status
    temp = SPIx->SPSR;
    dataCfg->counter = 0;
    dataCfg->status = 0;

    if(SPI_DATA_BIT_USED <= SPI_DATABIT_8)
        dataword = 0;
    else dataword = 1;

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
                SPI_SendData(SPIx, 0xFF);
            } else {
                SPI_SendData(SPIx, 0xFFFF);
            }
        } else {
            if (dataword == 0){
                SPI_SendData(SPIx, *wdata8);
                wdata8++;
            } else {
                SPI_SendData(SPIx, *wdata16);
                wdata16++;
            }
        }
        // Wait for transfer complete
        while (!((stat = SPIx->SPSR) & SPI_SPSR_SPIF))
        {
#if TIMEOUT_ENABLE
            timeout++;
            if(timeout >= SPI_RW_TIMEOUT)
            {
                dataCfg->status = SPI_STAT_ERROR;
                return dataCfg->counter;
            }
#endif
        }
        // Check for error
        //if (stat & (SPI_SPSR_ABRT | SPI_SPSR_MODF | SPI_SPSR_ROVR | SPI_SPSR_WCOL)){
        if (stat & (SPI_SPSR_ABRT | SPI_SPSR_MODF )){
            // save status
            dataCfg->status = stat | SPI_STAT_ERROR;
            return (dataCfg->counter);
        }
        // Read data from SPI dat
        temp = (uint32_t) SPI_ReceiveData(SPIx);

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

/********************************************************************//**
 * @brief 	Get current value of SPI Status register in SPIx peripheral.
 * @param[in]   SPIx	SPI peripheral selected, should be LPC_SPI
 * @return      Current value of SPI Status register in SPI peripheral.
 * Note:	The return value of this function must be used with
 * 			SPI_CheckStatus() to determine current flag status
 * 			corresponding to each SPI status type. Because some flags in
 * 			SPI Status register will be cleared after reading, the next reading
 * 			SPI Status register could not be correct. So this function used to
 * 			read SPI status register in one time only, then the return value
 * 			used to check all flags.
 *********************************************************************/
uint32_t SPI_GetStatus(LPC_SPI_TypeDef* SPIx)
{
    CHECK_PARAM(PARAM_SPIx(SPIx));

    return (SPIx->SPSR & SPI_SPSR_BITMASK);
}


/**
 * @}
 */


/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
