/******************************************************************************
*  Title    : LPC11xx Capacitive Touch Sensing demo program
*  Name     : Paul Seerden
*  Hardware : MicroCore48 board + PCF8883 evaluation kit
*
*  1. Use Systick timer to generate a 10 msec timer tick (interrupt driven).
*  2. Capacitive sense plate is connected to P1.0 = AD1 input
*  3. Every 10 msec: use ADC to read the capacitive sense input.
*  4. P3.3 = ON/OFF LED used to indicate press and release condition
*  5. P3.2 = toggle LED is used to indicate a new press condition
*
*                  (C) Copyright 2010 NXP Semiconductors
*
*******************************************************************************/

#include <LPC11xx.h>                               // LPC11xx definitions

#define ADC_DONE           0x80000000

static char k_press = 0;
static int  average = 0;


static short ADC_ReadCH1(void)                     // read ADC channel AD1
{
    LPC_IOCON->R_PIO1_0 = 2;                       // AD1/P1.0 = AD1 ~~~~~~~~~ set sensor as AD1 input
    LPC_ADC->CR = 2 |                              // SEL = 2, select channel 1 on ADC
                 (3 << 8) |                        // ADC_CLK = Fpclk / CLKDIV = 12 MHz/3 = 4 MHz
                 (1 << 24);                        // start conversion

    while (!(LPC_ADC->DR[1] & ADC_DONE)) ;         // wait until end of AD1 conversion

    LPC_IOCON->R_PIO1_0 = 0x81;                    // AD1/P0.1 = GPO ~~~~~~~~~ charge the cap plate
    LPC_ADC->CR &= 0xF8FFFFFF;                     // stop ADC

    return (LPC_ADC->DR[1] >> 6) & 0x3FF;          // return A/D conversion value
}

void SysTick_Handler(void)                         // SysTick Timer ISR every 10 msec
{
  static char  debounce = 0;
  static char  avgindex = 0;
         char  result   = 0;
         short reading;

    reading = ADC_ReadCH1();                       // read ADC channel 1 = Cap sense input

    if (reading > average + (average >> 4))        // above (average + 6% of average) -> press condition
    {
        if (debounce == 4)                         // debounce, 4 successive triggers for press
        {
            k_press = 1;                           // reached max, indicate pressed
            result = 1;                            // set result for return value
        }
        else
            debounce ++;                           // still going toward max	
    }
    else if (reading < average + (average >> 5))   // below (average + 3% of average) -> release condition
    {
        if (debounce == 0)
        {					
            k_press = 0;                           // reached min, indicate release
            result = 0;                            // clear result for return value
        }
        else
            debounce --;                           // going toward min
    }

    if (result == 0 && debounce == 0)              // recalculate average when not pressed
    {
        if (++avgindex == 8)                       // average index delay
        {
            average = (reading + (15 * average)) / 16;
			avgindex = 0;
        }
    }
}

int main (void)
{
  static char  toggle = 0;
  static char  ledon  = 0;
         short i;

    SystemInit();

    LPC_GPIO1->DIR |= (1<<0);                      // P1.0 = output connected to cap sense plate
    LPC_GPIO3->DIR |= (1<<2);                      // P3.2 = toggle LED
    LPC_GPIO3->DIR |= (1<<3);                      // P3.3 = ON/OFF LED

    LPC_SYSCON->PDRUNCFG      &= ~(1<<4);          // disable power-down bit to the ADC block
    LPC_SYSCON->SYSAHBCLKCTRL |=  (1<<13);         // enable AHB clock to the ADC

    for (i=0; i<200; i++)                          // warm up, establish average
    {
        average = (32 + ADC_ReadCH1() + (15 * average)) / 16;
    }

    SysTick_Config(SystemCoreClock/100);           // generate interrupt each 10 ms

    while (1)
    {
        __wfi();                                   // go to sleep

        if (k_press)                               // key pressed ?
        {
            LPC_GPIO3->DATA &= ~(1<<3);            // P3.3 low = LED ON
            if (!toggle)
            {
                toggle = 1;
                if (!ledon)
                {
                    ledon = 1;
                    LPC_GPIO3->DATA &= ~(1<<2);    // P3.2 low = LED ON
                }
                else
                {
                    ledon = 0;
                    LPC_GPIO3->DATA |= (1<<2);     // P3.2 high = LED OFF
                }
            }
        }
        else                                       // key released
        {
            LPC_GPIO3->DATA |= (1<<3);             // P3.3 high = LED OFF
            if (toggle)
            {
                toggle = 0;
            }
        }
    }
}
