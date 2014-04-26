/*****************************************************************************
 *   timer32.h:  Header file for NXP LPC13xx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.08.20  ver 1.00    Preliminary version, first Release
 *
******************************************************************************/
#ifndef __TIMER32_H 
#define __TIMER32_H

/* The test is either MAT_OUT or CAP_IN. Default is MAT_OUT. */
#define TIMER_MATCH		0

#define TIME_INTERVAL	(SystemCoreClock/100 - 1)
/* depending on the SystemFrequency and System AHB clock divider setting, 
if SystemFrequency = 60Mhz, SYSAHBCLKDIV = 4, SystemAHBFrequency = 1/4 SystemFrequency, 
10mSec = 150.000-1 counts */

extern volatile uint32_t timer32_0_counter;
extern volatile uint32_t timer32_1_counter;
extern volatile uint32_t timer32_0_capture;
extern volatile uint32_t timer32_1_capture;

void delay32Ms(uint8_t timer_num, uint32_t delayInMs);

#ifdef __IAR_SYSTEMS_ICC__
void CT32B0_IRQHandler(void);
void CT32B1_IRQHandler(void);
#else
void TIMER32_0_IRQHandler(void);
void TIMER32_1_IRQHandler(void);
#endif

void enable_timer32(uint8_t timer_num);
void disable_timer32(uint8_t timer_num);
void reset_timer32(uint8_t timer_num);
void init_timer32(uint8_t timer_num, uint32_t timerInterval);

#endif /* end __TIMER32_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
