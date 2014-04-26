/*****************************************************************************
 *   timer16.h:  Header file for NXP LPC13xx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.08.20  ver 1.00    Preliminary version, first Release
 *
******************************************************************************/
#ifndef __TIMER16_H 
#define __TIMER16_H

/* TIMER_CLOCKFREQ is the clock rate into the timer prescaler */
#define TIMER_CLOCKFREQ SystemCoreClock

/* MHZ_PRESCALE is a value to set the prescaler to in order to
   clock the timer at 1 MHz. Clock needs to be a multiple of 1 MHz or
   this will not work. */
#define MHZ_PRESCALE    (TIMER_CLOCKFREQ/1000000)

/* TIME_INTERVALmS is a value to load the timer match register with
   to get a 1 mS delay */
#define TIME_INTERVALmS	1000

/* The test is either MAT_OUT or CAP_IN. Default is MAT_OUT. */
#define TIMER_MATCH		0

void delayMs(uint8_t timer_num, uint32_t delayInMs);

#ifdef __IAR_SYSTEMS_ICC__
void CT16B0_IRQHandler(void);
void CT16B1_IRQHandler(void);
#else
void TIMER16_0_IRQHandler(void);
void TIMER16_1_IRQHandler(void);
#endif

void enable_timer16(uint8_t timer_num);
void disable_timer16(uint8_t timer_num);
void reset_timer16(uint8_t timer_num);
void init_timer16(uint8_t timer_num, uint16_t timerInterval);

#endif /* end __TIMER16_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
