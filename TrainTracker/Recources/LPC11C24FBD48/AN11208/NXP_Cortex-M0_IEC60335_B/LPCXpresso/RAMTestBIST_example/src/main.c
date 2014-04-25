/****************************************************************************
 *   $Id:: main.c 8481 2011-10-27 12:49:37Z nxp28536               $
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

volatile unsigned long SysTickCnt;      /* SysTick Counter                    */

void SysTick_Handler (void) {           /* SysTick Interrupt Handler (1ms)    */

	SysTickCnt++;
}

void Delay (unsigned long tick) {       /* Delay Function                     */
  unsigned long systickcnt;

  systickcnt = SysTickCnt;
  while ((SysTickCnt - systickcnt) < tick);
}



int main(void)
{ 
	UINT32 startAddr, endAddr, lenght;
    
    initLed(LED_BIT);

    SystemInit();
    SystemCoreClockUpdate();
    
    /* Generate interrupt each 1 ms   */
    SysTick_Config(SystemCoreClock/1000 - 1);


	/** Please see the ranges in LPC1114 or LPC1227 linker script !! */
	startAddr = 0x10000000;
	endAddr = 0x10000014;
	lenght = endAddr - startAddr;

    while(1)
    {
		
		/* note that for the sake of demonstration the table is pointing to unused ram */
		/* if used ram needs to be tested, user must backup content first ! */      
		if(IEC60335_RAMtest_BIST(startAddr, lenght) == IEC60335_testFailed)
		{
		  /* RAM BIST test failed */
		  while (1);
		}  
		
		Delay(500);
		toggleLed(LED_BIT);
	}
}
