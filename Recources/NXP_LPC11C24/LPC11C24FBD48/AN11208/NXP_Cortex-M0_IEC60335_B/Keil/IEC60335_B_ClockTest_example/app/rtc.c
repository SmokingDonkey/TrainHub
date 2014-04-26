#include "lpc12xx.h"
#include "IEC60335.h"

#define RTC_MATCH    1023                      // 1 seconds


void RTC_IRQHandler(void)
{
    IEC60335_Clocktest_RTCHandler();

    LPC_RTC->ICR = 0x01;                       // clear RTC interrupt
    LPC_RTC->LR  = 0x00;                       // reload RTC initial value
}

void RTC_Init(void)
{
    LPC_PMU->GPREG4 &= ~(0xF << 11);
    LPC_PMU->GPREG4 |= 0x0B << 11;             // 1Khz from RTC clock
//  LPC_PMU->GPREG4 |= 0x01 << 11;	/* 1hz from RTC clock */

    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<19);      // enable clock to RTC

    LPC_RTC->LR = 0x00;                        // load RTC
    LPC_RTC->MR = RTC_MATCH;                   // set match value

    NVIC_EnableIRQ(RTC_IRQn);                  // enable the RTC Interrupt

    LPC_RTC->IMSC = 0x01;
    LPC_RTC->CR   = 0x1;                       // start RTC
}
