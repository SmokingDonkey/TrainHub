/****************************************************************************
 *   $Id:: systick.h 3632 2010-06-01 22:54:42Z usb00423                     $
 *   Project: NXP LPC13xx software example
 *
 *   Description:
 *     This file contains definition and prototype for systick configuration.
 *
 ****************************************************************************
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
****************************************************************************/
#ifndef __SYSTICK_H 
#define __SYSTICK_H

/* SysTick clock source */
#define SysTick_CLKSource_RCLK         (0xFFFFFFFB)
#define SysTick_CLKSource_CCLK         (0x00000004)

/* SysTick counter state */
#define SysTick_Counter_Disable        (0xFFFFFFFE)
#define SysTick_Counter_Enable         (0x00000001)
#define SysTick_Counter_Clear          (0x00000000)

/* CTRL TICKINT Mask */
#define CTRL_TICKINT_Set               (0x00000002)
#define CTRL_TICKINT_Reset             (0xFFFFFFFD)

/* SysTick Flag */
#define SysTick_FLAG_COUNT             (0x30)
#define SysTick_FLAG_SKEW              (0x5E)
#define SysTick_FLAG_NOREF             (0x5F)

/* Exported functions ------------------------------------------------------- */
void SysTick_Handler(void);
void delaySysTick(uint32_t tick);
void SysTick_CLKSourceConfig(uint32_t SysTick_CLKSource);
void SysTick_SetReload(uint32_t Reload);
void SysTick_CounterCmd(uint32_t SysTick_Counter);
void SysTick_ITConfig(FunctionalState NewState);
uint32_t SysTick_GetCounter(void);
FlagStatus SysTick_GetFlagStatus(uint8_t SysTick_FLAG);

#endif /* end __SYSTICK_H */

/******************************************************************************
**                            End Of File
******************************************************************************/
