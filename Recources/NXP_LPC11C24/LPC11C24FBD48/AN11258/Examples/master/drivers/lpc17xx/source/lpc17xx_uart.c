/**********************************************************************
* $Id$         lpc17xx_uart.c            2011-06-06
*//**
* @file        lpc17xx_uart.c
* @brief       Contains all functions support for UART firmware library
*              on LPC17xx
* @version     3.2
* @date        25. July. 2011
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

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup UART
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_uart.h"
#include "lpc17xx_clkpwr.h"

/* Public Functions ----------------------------------------------------------- */
/** @addtogroup UART_Public_Functions
 * @{
 */
/* UART Init/DeInit functions -------------------------------------------------*/
/********************************************************************//**
 * @brief        Initializes the LPC_UART0 peripheral according to the specified
 *               parameters in the UART_ConfigStruct.
 * @param[in]    None
 * @return       None
 *********************************************************************/
void UART_Init(void)
{
    /* Set up clock and power for UART module */
    LPC_SC->PCONP |= CLKPWR_PCONP_PCUART0;

    // Disable FIFO
    LPC_UART0->FCR = 0;
    // Disable interrupt
    LPC_UART0->IER = 0;
    // Set ACR to default state
    LPC_UART0->ACR = 0;

    // Set divisors
    LPC_UART0->LCR = UART_LCR_DLAB_EN;
    LPC_UART0->DLM = UART_LOAD_DLM(0x0A);
    LPC_UART0->DLL = UART_LOAD_DLL(0x0A);
    /* Then reset DLAB bit */
    LPC_UART0->LCR &= (~UART_LCR_DLAB_EN) ;
    LPC_UART0->FDR = (UART_FDR_MULVAL(0x0E) \
            | UART_FDR_DIVADDVAL(0x05)) & UART_FDR_BITMASK;

    // Set Line Control register ----------------------------
    LPC_UART0->LCR = (LPC_UART0->LCR & (UART_LCR_DLAB_EN | UART_LCR_BREAK_EN))|UART_LCR_WLEN8;
}

/*********************************************************************//**
 * @brief        De-initializes the LPC_UART0 peripheral registers to their
 *                  default reset values.
 * @param[in]    None
 * @return       None
 **********************************************************************/
void UART_DeInit(void)
{
    // TX Disable
    LPC_UART0->TER &= (~UART_TER_TXEN);

    /* Set up clock and power for UART module */
    LPC_SC->PCONP &= ~CLKPWR_PCONP_PCUART0;
}

/* UART Send/Recieve functions -------------------------------------------------*/

/*********************************************************************//**
 * @brief        Send a block of data via UART peripheral
 * @param[in]    txbuf     Pointer to Transmit buffer
 * @param[in]    buflen     Length of Transmit buffer
 * @return       Number of bytes sent.
 *
 * Note: when using UART in BLOCKING mode, a time-out condition is used
 * via defined symbol UART_BLOCKING_TIMEOUT.
 **********************************************************************/
uint32_t UART_Send(uint8_t *txbuf, uint32_t buflen)
{
    uint32_t bSent, timeOut, fifo_cnt;

    // blocking mode
    bSent = 0;
    while (bSent < buflen){
        timeOut = UART_BLOCKING_TIMEOUT;
        // Wait for THR empty with timeout
        while (!(LPC_UART0->LSR & UART_LSR_THRE)) {
            if (timeOut == 0)
                goto send_end;
            timeOut--;
        }
        fifo_cnt = UART_TX_FIFO_SIZE;
        while (fifo_cnt && (bSent<buflen)){
            LPC_UART0->THR = txbuf[bSent] & UART_THR_MASKBIT;
            fifo_cnt--;
            bSent++;
        }
    }
send_end:
    return bSent;
}

/*********************************************************************//**
 * @brief        Receive a block of data via UART peripheral
 * @param[out]     rxbuf     Pointer to Received buffer
 * @param[in]    buflen     Length of Received buffer
 * @return       Number of bytes received
 *
 * Note: when using UART in BLOCKING mode, a time-out condition is used
 * via defined symbol UART_BLOCKING_TIMEOUT.
 **********************************************************************/
uint32_t UART_Receive( uint8_t *rxbuf, uint32_t buflen)
{
    uint32_t bRecv, timeOut;

    bRecv = 0;
    while (bRecv < buflen){
        timeOut = UART_BLOCKING_TIMEOUT;
        while (!(LPC_UART0->LSR & UART_LSR_RDR)){
            if (timeOut == 0)
                goto rcv_end;
            timeOut--;
        }
        // Get data from the buffer
        rxbuf[bRecv] = (LPC_UART0->RBR & UART_RBR_MASKBIT);
        bRecv++;
    }
rcv_end:    
    return bRecv;
}

/********************************************************************//**
 * @brief        Get current value of Line Status register in UART peripheral.
 * @param[in]    None
 * @return       Current value of Line Status register in UART peripheral.
 * Note:         The return value of this function must be ANDed with each member in
 *                      UART_LS_Type enumeration to determine current flag status
 *                      corresponding to each Line status type. Because some flags in
 *                      Line Status register will be cleared after reading, the next reading
 *                      Line Status register could not be correct. So this function used to
 *                      read Line status register in one time only, then the return value
 *                      used to check all flags.
 *********************************************************************/
uint8_t UART_GetLineStatus(void)
{
    return ((LPC_UART0->LSR) & UART_LSR_BITMASK);
}


/**
 * @}
 */

/**
 * @}
 */
/* --------------------------------- End Of File ------------------------------ */

