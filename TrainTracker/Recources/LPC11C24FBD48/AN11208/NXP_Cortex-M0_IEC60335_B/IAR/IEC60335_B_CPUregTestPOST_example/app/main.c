/****************************************************************************
 *   $Id:: main.c 8339 2011-10-17 12:41:31Z nxp28536                        $
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
#include "IEC60335.h"
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
    initLed(LED_BIT);

    SystemInit();
    SystemCoreClockUpdate();
    
    /* Generate interrupt each 1 ms   */
    SysTick_Config(SystemCoreClock/1000 - 1);
    
    /* code will get here if test passed */
    while(1)
    {
      Delay(500);
      toggleLed(LED_BIT);     
    };
}
