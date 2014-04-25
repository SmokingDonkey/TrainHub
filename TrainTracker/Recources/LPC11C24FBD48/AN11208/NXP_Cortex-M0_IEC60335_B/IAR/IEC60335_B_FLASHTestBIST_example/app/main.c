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
#include "IEC60335.h"


volatile unsigned long SysTickCnt;      /* SysTick Counter */
extern type_testResult _FLASHTestBIST (void);

#ifdef LPC1114
FlashSign_t SoftflashSignature;
#endif


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
#ifdef LPC1114
    UINT32  begin, *end;
    UINT32 length;
#endif
  
    SystemInit();
    SystemCoreClockUpdate();
  
    /* Initialize the LED indicator */  
    initLed(LED_BIT);
    
    /* Generate interrupt each 1 ms   */
    SysTick_Config(SystemCoreClock/1000 - 1);
    
#ifdef LPC1114
    end = IEC60335_TOP_ROM_POST;
    begin = (UINT32) &IEC60335_BOTTOM_ROM_POST;
    length = (UINT32) end - begin;
    StartSoftSignatureGen( begin,                         /* Start address */
                           length, /* Length */
                           &SoftflashSignature );         /* Result structure pointer */
#endif
    
    /* code will get here if test passed */
    while(1)
    {
      /* Execute the ASM version of the Flash BIST test 
       * If the test fails, it will hook to _flashPostTestFailureHook
       */
      _FLASHTestBIST();
     
      Delay(500);
      toggleLed(LED_BIT);     
    };
}
