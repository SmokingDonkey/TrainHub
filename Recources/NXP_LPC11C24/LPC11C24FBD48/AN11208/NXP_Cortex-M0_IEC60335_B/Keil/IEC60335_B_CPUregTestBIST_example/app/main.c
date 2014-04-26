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
#elif defined (LPC1227)
	#include "LPC12xx.h"
	#include "lpc12xx_led.h"
#endif

#include "IEC60335.h"
#include "IEC60335_B_UserData.h"

volatile unsigned long SysTickCnt;      /* SysTick Counter                    */
volatile int led = 1;
extern type_testResult CPUregTestPOSTStatus;

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
    initLed(LED_BIT);

    SystemInit();
    SystemCoreClockUpdate();
    
    /* Generate interrupt each 1 ms   */
    SysTick_Config(SystemCoreClock/1000 - 1);
    
    /* code will get here if test passed */
    while(1)
    {
      /* Do the IEC60335 CPU register tests */
      /* Clear up the test structure */
      CPUregTestBIST_struct.testPassed = IEC60335_testFailed;
      CPUregTestBIST_struct.testState = 0x0;      
      _CPUregTestLOW();
      if (CPUregTestBIST_struct.testPassed == IEC60335_testFailed)
      {
          /* BIST CPU register test failed */
          while (1);
      }

      /* Clear up the test structure */
      CPUregTestBIST_struct.testPassed = IEC60335_testFailed;
      CPUregTestBIST_struct.testState = 0x0;      
      _CPUregTestMID();
      if (CPUregTestBIST_struct.testPassed == IEC60335_testFailed)
      {
          /* BIST CPU register test failed */
          while (1);
      }

      /* Clear up the test structure */
      CPUregTestBIST_struct.testPassed = IEC60335_testFailed;
      CPUregTestBIST_struct.testState = 0x0;      
      _CPUregTestHIGH();
      if (CPUregTestBIST_struct.testPassed == IEC60335_testFailed)
      {
          /* BIST CPU register test failed */
          while (1);
      }
      
      /* Clear up the test structure */
      CPUregTestBIST_struct.testPassed = IEC60335_testFailed;
      CPUregTestBIST_struct.testState = 0x0;      
      _CPUregTestSP();
      if (CPUregTestBIST_struct.testPassed == IEC60335_testFailed)
      {
          /* BIST CPU register test failed */
          while (1);
      }

      /* Clear up the test structure */
      CPUregTestBIST_struct.testPassed = IEC60335_testFailed;
      CPUregTestBIST_struct.testState = 0x0;      
      _CPUregTestSPEC();
      if (CPUregTestBIST_struct.testPassed == IEC60335_testFailed)
      {
          /* BIST CPU register test failed */
          while (1);
      }      

      Delay(500);
      toggleLed(LED_BIT);      
    };
}
