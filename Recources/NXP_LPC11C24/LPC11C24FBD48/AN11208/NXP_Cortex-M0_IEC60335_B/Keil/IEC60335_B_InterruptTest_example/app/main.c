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
#ifdef LPC1114 //--> Defined by selected Target
	#include "LPC11xx.h"
	#include "lpc11xx_led.h"
#endif
#ifdef LPC1227
	#include "LPC12xx.h"
	#include "lpc12xx_led.h"
#endif

#include "IEC60335.h"
#include "IEC60335_B_UserData.h"

extern void CT32B0_Init(uint32_t interval);

type_InterruptTest CT32B0_IntTest;
volatile unsigned char clock_100ms;


void SysTick_Handler(void) {
  static unsigned long ticks;

    if (ticks++ >= 9)
    {
        ticks       = 0;
        clock_100ms = 1;
    }
}

int main(void)
{

    /* Initialize the system */
	SystemInit();
	initLed (LED_BIT);

	/* Generate IRQ each 10 ms */
	SysTick_Config(SystemCoreClock/100);           

	/* generate Timer32_0 interrput every 1 msec */
    CT32B0_Init(12000);                            
	
	/*********************************************
	void IEC60335_InitInterruptTest(type_InterruptTest *pIRQ, UINT32 lowerBound, UINT32 upperBound, UINT32 individualValue)
	
	  Used:
	    SysTick timer  : every 10 msec
		Timer32_0:       every 1 msec (this is the interrupt we want to test)
	
	  Main loop Timer32_0 Interrupt check is called every ~100 msec (derived from SysTick)
	  Within 100 msec, 100 Timer32_0 interrupts should have occurred
	  So:
	    lowerBound      = 99  (check for more interrupts than lower bound)
	    upperBound      = 101 (check for less interrupts than upper bound)
	    individualValue = 1   (interrupt up-counting value)

	********************************************/
    IEC60335_InitInterruptTest(&CT32B0_IntTest, 99, 101, 1);

    while (1)
    {
	    if (clock_100ms)
        {
            clock_100ms = 0;
            /* toggle green LED P0.7 */
			toggleLed(LED_BIT);            

            /* Periodic IEC60335 Class B testing */
            if (IEC60335_InterruptCheck(&CT32B0_IntTest) == IEC60335_testFailed)
            {
                setLed(LED_BIT);
				while(1);
            }
        }
    }
}
