/****************************************************************************
 *   $Id:: IEC60335_B_CPUregTestBIST_ARM.s 7461 2011-06-01 13:01:58Z gerrit#$
 *   Project: Example for the NXP Cortex-M0 IEC60335 Class B certified library	
 *
 *   Description:
 *     Main source file
 *	    
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

#include "LPC12xx.h"
#include "lpc12xx_led.h"

#include "rtc.h"
#include "IEC60335.h"
#include "IEC60335_B_UserData.h"

extern void CT32B0_Init(uint32_t interval);
type_InterruptTest CT32B0_IntTest;
volatile unsigned char clock_100ms;


void SysTick_Handler(void)
{
  static unsigned long ticks;
	
	IEC60335_Clocktest_TimerIntHandler();

    if (ticks++ >= 9)
    {
        ticks       = 0;
        clock_100ms = 1;
    }
}

int main(void)
{

    initLed (LED_BIT);
    
    /* Generate IRQ each 10 ms */
    SysTick_Config(SystemCoreClock/100);

    /* Intialize the RTC */
    RTC_Init();
    
    /*
      IEC60335_initClockTest(timerOccThreshold, rtcOccThreshold, timerLowerBound, timerUpperBound);
    
      Used:
        SysTick timer  : every 10 msec
            Real Time Clock: every 1 sec
            Main loop clock test is called every ~100 msec
    
      So:
        timerOccThreshold = 1  (within 100 msec always more than 1 systick interrupt has occurred
        rtcOccThreshold   = 11 (need 11 x 100 msec to have at least 1 RTC interrupt
    
      Within 1 RTC interrupt 100 systick interrupts should have occurred
      So:
        timerLowerBound = 99  (more timer interrupts than lower bound)
        timerUpperBound = 101 (less timer interrupts than upper bound)
    
    */
    IEC60335_initClockTest(1, 11, 99, 101);

    while (1)
    {
        if (clock_100ms)
        {
            clock_100ms = 0;
            /* toggle LED on P0.7 */
			toggleLed(LED_BIT);

            /* Periodic IEC60335 Class B testing */
            if (IEC60335_Clocktest_MainLoopHandler() == IEC60335_testFailed)
            {
                setLed(LED_BIT); // TEST FAIL: LED will stay on
				while(1);
            }
        }
    }
}
