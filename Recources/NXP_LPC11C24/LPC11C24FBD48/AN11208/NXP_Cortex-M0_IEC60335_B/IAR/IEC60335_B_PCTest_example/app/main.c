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
#ifdef LPC1227 
  #include "LPC12xx.h"
#elif LPC1114
  #include "LPC11xx.h"
#endif

#include "IEC60335.h"

#ifdef LPC1227 
  #include "lpc12xx_led.h"
#elif LPC1114
  #include "lpc11xx_led.h"
#endif

#include "IEC60335_B_UserData.h"

/* SysTick Counter */
volatile unsigned long SysTickCnt;      

/* SysTick Interrupt Handler (1ms)    */
void SysTick_Handler (void) {          
	SysTickCnt++;
}

/* simple delay function */
void Delay (unsigned long tick) 
{       
  unsigned long systickcnt;

  systickcnt = SysTickCnt;
  while ((SysTickCnt - systickcnt) < tick);
}


int main(void)
{ 
    
    /* Run the Program Counter POST test */  
    if (IEC60335_B_PCTest_POST() == IEC60335_testFailed)
    {
        /* The PC POST test failed */

        /* Put your code here to handle this failure.. */
        while(1);
    }
  
    /* Initialize the LED */
    initLed(LED_BIT);

    /* Initialize the system */
    SystemInit();
    SystemCoreClockUpdate();
    
    /* Generate interrupt each 1 ms   */
    SysTick_Config(SystemCoreClock/1000 - 1);

    while(1)
    {
        /* Run the Program Counter POST test */  
        if (IEC60335_B_PCTest_BIST() == IEC60335_testFailed)
        {
                /* The PC POST test failed */
        
                /* Put your code here to handle this failure.. */
                while(1);
        }
      
      Delay(500);
      toggleLed(LED_BIT);     

    }
}

