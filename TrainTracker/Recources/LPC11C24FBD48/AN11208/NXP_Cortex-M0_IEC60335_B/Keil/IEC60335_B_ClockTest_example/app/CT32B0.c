#ifdef LPC1114
	#include "LPC11xx.h"
#endif
#ifdef LPC1227
	#include "LPC12xx.h"
#endif
#include "IEC60335.h"


extern type_InterruptTest CT32B0_IntTest;

void TIMER32_0_IRQHandler(void)
{  
    LPC_TMR32B0->IR = 1;                           // clear interrupt flag
    IEC60335_InterruptOcurred(&CT32B0_IntTest);    // for Class B testing
}

void CT32B0_Init(uint32_t interval) 
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<9);

    LPC_IOCON->PIO2_4 |= 3;                        // Timer32_0 MAT0

    LPC_TMR32B0->MR0  = interval;

    LPC_TMR32B0->EMR |= (3<<4);                    // MR0 Toggle
    LPC_TMR32B0->MCR  = 3;                         // Interrupt and Reset on MR0

    NVIC_EnableIRQ(TIMER_32_0_IRQn);
    LPC_TMR32B0->TCR = 1;                          // start timer
}
