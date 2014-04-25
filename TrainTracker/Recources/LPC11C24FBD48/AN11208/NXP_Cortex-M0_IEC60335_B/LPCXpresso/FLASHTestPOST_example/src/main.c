/****************************************************************************
 *   $Id:: main.c 8009 2011-08-31 08:22:38Z nxp28536                        $
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

#include "LPC11xx.h"
#include "lpc11xx_led.h"

#include "IEC60335.h"
#include "IEC60335_B_UserData.h"

extern type_testResult  FlashPostTestStatus;

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
