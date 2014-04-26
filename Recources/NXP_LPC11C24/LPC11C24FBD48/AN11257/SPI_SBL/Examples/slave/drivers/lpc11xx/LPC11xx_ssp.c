/**
 * @file	: lpc11xx_ssp.c
 * @brief	: Contains all functions support for SSP firmware library on LPC11xx
 * @version	: 1.0
 * @date	: 9. April. 2009
 * @author	: HieuNguyen
 * @modify	: 08. Jan. 2010 - NguyenCao
 **************************************************************************
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
 **********************************************************************/
/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup SSP
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc11xx_ssp.h"


/* Private Variables ------------------------------------------------------------------- */
/** @defgroup 	SSP_Private_Variables
 * @{
 */

#define CLKPWR_PCONP_PCSSP                    (1<<8)
#define SSP_CPHA_USED                         (SSP_CPHA_SECOND)
#define SSP_CPOL_USED                         (SSP_CPOL_LO)
#define SSP_CLK_RATE_USED                      (1000000)
#define SSP_DATA_BIT_USED                      (SSP_DATABIT_8)
#define SSP_MODE_USED                          (SSP_SLAVE_MODE)



/**
 * @}
 */

/* Private Functions ------------------------------------------------------------------- */
/** @defgroup     SSP_Private_Functions
 * @{
 */


 /**
 * @}
 */


/* Public Functions ------------------------------------------------------------------- */
/** @defgroup SSP_Public_Functions
 * @{
 */

/*********************************************************************//**
 * @brief     De-initializes the ssp_dev peripheral registers to their
*                  default reset values.
 * @param[in] ssp_dev	SSP peripheral selected, should be SSP0 or SSP1
 * @return    None
 **********************************************************************/
void SSP_DeInit(LPC_SSP_TypeDef* ssp_dev)
{
    if(ssp_dev == LPC_SSP0)
        LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<11);
    else
        LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<18);
}



/********************************************************************//**
 * @brief      Initializes the ssp_dev peripheral according to the specified
 *               parameters in the SSP_ConfigStruct.
 * @param[in]  ssp_dev	SSP peripheral selected, should be SSP0 or SSP1
 * @return 	    	None
 *********************************************************************/
void SSP_Init(LPC_SSP_TypeDef *ssp_dev)
{
    uint32_t tmp;
    // initialize pinsel for SSP0
    /* MISO0: P0.8 */
    LPC_IOCON->PIO0_8 = 0x01 | (0x02<<3);
    /* MOSI0: P0.9 */
    LPC_IOCON->PIO0_9 = 0x01 | (0x02<<3);

    /* Selects SCK0 pin*/
    LPC_IOCON->SCK_LOC = 0x02; //PIO0_6/SCK0
    LPC_IOCON->PIO0_6 = 0x02 | (0x02<<3);

    /* SSEL0 : P0.2*/
    LPC_IOCON->PIO0_2 &= ~0x1F;        
    LPC_IOCON->PIO0_2 |= 0x01 | (0x02<<3);        

    /* Reset SSP0 peripheral */
    LPC_SYSCON->PRESETCTRL |= (0x1<<0);
    /* Enable clock for SSP0 */
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<11);
    LPC_SYSCON->SSP0CLKDIV = 0x02;
    

    /* Configure SSP, interrupt is disable, LoopBack mode is disable,
     * SSP is disable, Slave output is disable as default
     */
    tmp = ((SSP_CPHA_USED) | (SSP_CPOL_USED) \
        | (SSP_FRAME_SPI) | (SSP_DATA_BIT_USED))
        & SSP_CR0_BITMASK;
    // write back to SSP control register
    ssp_dev->CR0 = tmp;

    // Write back to CR1
    ssp_dev->CR1 = SSP_SLAVE_MODE;

    // Set clock rate for SSP peripheral
    ssp_dev->CR0 &= (~ SSP_CR0_SCR (0xFF)) & SSP_CR0_BITMASK;
    ssp_dev->CR0 |= (SSP_CR0_SCR(0x05)) & SSP_CR0_BITMASK;
    ssp_dev->CPSR = 0x02 & SSP_CPSR_BITMASK;
}


/*********************************************************************//**
 * @brief       Enable or disable SSP peripheral's operation
 * @param[in]   ssp_dev	SSP peripheral, should be SSP0 or SSP1
 * @param[in]   NewState New State of ssp_dev peripheral's operation
 * @return      none
 **********************************************************************/
void SSP_Cmd(LPC_SSP_TypeDef* ssp_dev, FunctionalState NewState)
{

    if (NewState == ENABLE)
    {
        ssp_dev->CR1 |= SSP_CR1_SSP_EN;
    }
    else
    {
        ssp_dev->CR1 &= (~SSP_CR1_SSP_EN) & SSP_CR1_BITMASK;
    }
}




/*********************************************************************//**
 * @brief      Enable or disable Slave Output function in SSP peripheral
 * @param[in]  ssp_dev	SSP peripheral selected, should be SSP0 or SSP1
 * @param[in]  NewState	New State of Slave Output function, should be:
 * 			   - ENABLE: Slave Output in normal operation
 * 			   - DISABLE: Slave Output is disabled. This blocks
 * 		       SSP controller from driving the transmit data line (MISO)
 * Note:       This function is available when SSP peripheral in Slave mode
 * @return     None
 **********************************************************************/
void SSP_SlaveOutputCmd(LPC_SSP_TypeDef* ssp_dev, FunctionalState NewState)
{

    if (NewState == ENABLE)
    {
        ssp_dev->CR1 &= (~SSP_CR1_SO_DISABLE) & SSP_CR1_BITMASK;
    }
    else
    {
        ssp_dev->CR1 |= SSP_CR1_SO_DISABLE;
    }
}



/*********************************************************************//**
 * @brief     Transmit a single data through ssp_dev peripheral
 * @param[in] ssp_dev	SSP peripheral selected, should be SSP
 * @param[in] Data	Data to transmit (must be 16 or 8-bit long,
 * 			this depend on SSP data bit number configured)
 * @return    none
 **********************************************************************/
void SSP_SendData(LPC_SSP_TypeDef* ssp_dev, uint16_t Data)
{

    ssp_dev->DR = SSP_DR_BITMASK(Data);
}



/*********************************************************************//**
 * @brief      Receive a single data from ssp_dev peripheral
 * @param[in]  ssp_dev	SSP peripheral selected, should be SSP
 * @return     Data received (16-bit long)
 **********************************************************************/
uint16_t SSP_ReceiveData(LPC_SSP_TypeDef* ssp_dev)
{

    return ((uint16_t) (SSP_DR_BITMASK(ssp_dev->DR)));
}

/*********************************************************************//**
 * @brief       SSP Read write data function
 * @param[in]   ssp_dev 	Pointer to SSP peripheral, should be SSP0 or SSP1
 * @param[in]   dataCfg	Pointer to a SSP_DATA_SETUP_Type structure that
 *                   contains specified information about transmit
 *                   data configuration.
 * @return      Actual Data length has been transferred in polling mode.
 *                   In interrupt mode, always return (0)
 *                   Return (-1) if error.
 * Note: This function can be used in both master and slave mode.
 ***********************************************************************/
int32_t SSP_ReadWrite (LPC_SSP_TypeDef *ssp_dev, SSP_DATA_SETUP_Type *dataCfg)
{
    uint8_t *rdata8;
    uint8_t *wdata8;
    uint16_t *rdata16;
    uint16_t *wdata16;
    uint32_t stat;
    uint32_t tmp;
//    int32_t sspnum;
    int32_t dataword;

    dataCfg->rx_cnt = 0;
    dataCfg->tx_cnt = 0;
    dataCfg->status = 0;


    /* Clear all remaining data in RX FIFO */
    while (ssp_dev->SR & SSP_SR_RNE){
        tmp = (uint32_t) SSP_ReceiveData(ssp_dev);
    }

    // Clear status
    ssp_dev->ICR = SSP_ICR_BITMASK;

    if (SSP_DATA_BIT_USED > SSP_DATABIT_8)
        dataword = 1;
    else
        dataword = 0;

    // Polling mode ----------------------------------------------------------------------
    {
        if (dataword == 0){
            rdata8 = (uint8_t *)dataCfg->rx_data;
            wdata8 = (uint8_t *)dataCfg->tx_data;
        } else {
            rdata16 = (uint16_t *)dataCfg->rx_data;
            wdata16 = (uint16_t *)dataCfg->tx_data;
        }
        while ((dataCfg->tx_cnt != dataCfg->length) || (dataCfg->rx_cnt != dataCfg->length)){
            if ((ssp_dev->SR & SSP_SR_TNF) && (dataCfg->tx_cnt != dataCfg->length)){
                // Write data to buffer
                if(dataCfg->tx_data == NULL){
                    if (dataword == 0){
                        SSP_SendData(ssp_dev, 0xFF);
                        dataCfg->tx_cnt++;
                    } else {
                        SSP_SendData(ssp_dev, 0xFFFF);
                        dataCfg->tx_cnt += 2;
                    }
                } else {
                    if (dataword == 0){
                        SSP_SendData(ssp_dev, *wdata8);
                        wdata8++;
                        dataCfg->tx_cnt++;
                    } else {
                        SSP_SendData(ssp_dev, *wdata16);
                        wdata16++;
                        dataCfg->tx_cnt += 2;
                    }
                }
            }

            // Check overrun error
            if ((stat = ssp_dev->RIS) & SSP_RIS_RORRIS){
                // save status and return
                dataCfg->status = stat | SSP_STAT_ERROR;
                return (-1);
            }

            // Check for any data available in RX FIFO
            while ((ssp_dev->SR & SSP_SR_RNE) && (dataCfg->rx_cnt != dataCfg->length)){
                // Read data from SSP data
                tmp = SSP_ReceiveData(ssp_dev);

                // Store data to destination
                if (dataCfg->rx_data != NULL)
                {
                    if (dataword == 0){
                        *(rdata8) = (uint8_t) tmp;
                        rdata8++;
                    } else {
                        *(rdata16) = (uint16_t) tmp;
                        rdata16++;
                    }
                }
                // Increase counter
                if (dataword == 0){
                    dataCfg->rx_cnt++;
                } else {
                    dataCfg->rx_cnt += 2;
                }
            }
        }

        // save status
        dataCfg->status = SSP_STAT_DONE;

        if (dataCfg->tx_data != NULL){
            return dataCfg->tx_cnt;
        } else if (dataCfg->rx_data != NULL){
            return dataCfg->rx_cnt;
        } else {
            return (0);
        }
    }
}

/*********************************************************************//**
 * @brief       Checks whether the specified SSP status flag is set or not
 * @param[in]   ssp_dev	SSP peripheral selected, should be SSP0 or SSP1
 * @param[in]   FlagType	Type of flag to check status, should be one
 * 				of following:
 * 				- SSP_STAT_TXFIFO_EMPTY: TX FIFO is empty
 * 				- SSP_STAT_TXFIFO_NOTFULL: TX FIFO is not full
 * 				- SSP_STAT_RXFIFO_NOTEMPTY: RX FIFO is not empty
 * 				- SSP_STAT_RXFIFO_FULL: RX FIFO is full
 * 				- SSP_STAT_BUSY: SSP peripheral is busy
 * @return      New State of specified SSP status flag
 **********************************************************************/
FlagStatus SSP_GetStatus(LPC_SSP_TypeDef* ssp_dev, uint32_t FlagType)
{

    return ((ssp_dev->SR & FlagType) ? SET : RESET);
}
/**
 * @}
 */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
