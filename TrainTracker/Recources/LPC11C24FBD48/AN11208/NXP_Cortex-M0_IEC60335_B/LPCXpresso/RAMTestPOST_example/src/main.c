
/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
#if defined (LPC1114)
	#include "LPC11xx.h"
	#include "lpc11xx_led.h"
#elif defined (LPC1227)
	#include "LPC12xx.h"
	#include "lpc12xx_led.h"
#endif

#include "IEC60335.h"
#include "IEC60335_B_UserData.h"

//#define LED_GPIO    LPC_GPIO0
//#define LED_BIT     7
//#define CHANGE_BIT(x) (1<<x)
//
//#define setLed()    (LED_GPIO->DATA |= CHANGE_BIT(LED_BIT))
//#define clearLed()  (LED_GPIO->DATA &= (~(CHANGE_BIT(LED_BIT))))


volatile unsigned long SysTickCnt;      /* SysTick Counter                    */
volatile int led = 1;
extern type_testResult CPUregTestPOSTStatus;

//void initLed(void)
//{
//        /* reuse P0.7 led on LPC1200 LpcXpresso stick */
//        LED_GPIO->DIR |= CHANGE_BIT(LED_BIT);
//}
//
//void toggleLed(void)
//{
//  if(LED_GPIO->DATA & CHANGE_BIT(LED_BIT)) { clearLed(); }
//        else setLed();
//}

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
