/*****************************************************************************
 *   adc.c:  ADC C file for NXP LPC13xx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.07.20  ver 1.00    Preliminary version, first Release
 *
*****************************************************************************/
#include "LPC13xx.h"			/* LPC13xx Peripheral Registers */
#include "adc.h"

volatile uint32_t ADCValue[ADC_NUM];
volatile uint32_t ADCIntDone = 0;

#if BURST_MODE
volatile uint32_t channel_flag; 
#endif

#if ADC_INTERRUPT_FLAG
/******************************************************************************
** Function name:		ADC_IRQHandler
**
** Descriptions:		ADC interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void ADC_IRQHandler (void) 
{
  uint32_t regVal;
  uint32_t i;
  volatile uint32_t Dummy;
  
  regVal = LPC_ADC->STAT;		/* Read ADC will clear the interrupt */
  if ( regVal & 0x0000FF00 )	/* check OVERRUN error first */
  {
	regVal = (regVal & 0x0000FF00) >> 0x08;
	/* if overrun, just read ADDR to clear */
	/* regVal variable has been reused. */
    for(i=0; i<8; i++)
	{
		if(regVal & (1<<i))
			Dummy = LPC_ADC->DR[i];
	}
	LPC_ADC->CR &= 0xF8FFFFFF;	/* stop ADC now */ 
	ADCIntDone = 1;
	return;	
  }

  if ( regVal & ADC_ADINT )
  {
    for(i=0; i<8; i++) 	/* check DONE bits */
	{
		if(regVal & (1<<i))
			ADCValue[i] = LPC_ADC->DR[i];
	}
#if BURST_MODE
	channel_flag |= (regVal & 0xFF);
	if ( (channel_flag & 0xFF) == 0xFF )
	{
	  /* All the bits in have been set, it indicates all the ADC 
	  channels have been converted. */
	  LPC_ADC->CR &= 0xF8FFFFFF;	/* stop ADC now */ 
	  ADCIntDone = 1;
	}
#else
	LPC_ADC->CR &= 0xF8FFFFFF;	/* stop ADC now */ 
	ADCIntDone = 1;
#endif
  }
  return;
}
#endif

/*****************************************************************************
** Function name:		ADCInit
**
** Descriptions:		initialize ADC channel
**
** parameters:			ADC clock rate
** Returned value:		None
** 
*****************************************************************************/
void ADCInit( uint32_t ADC_Clk )
{
  /* Disable Power down bit to the ADC block. */  
  LPC_SYSCON->PDRUNCFG &= ~(0x1<<4);

  /* Enable AHB clock to the ADC. */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<13);

  /* Unlike some other pins, for ADC test, all the pins need
  to set to analog mode. Bit 7 needs to be cleared */

  LPC_IOCON->JTAG_TDI_PIO0_11   = 0x02;	// Select AD0 pin function
  LPC_IOCON->JTAG_TMS_PIO1_0    = 0x02;	// Select AD1 pin function
  LPC_IOCON->JTAG_TDO_PIO1_1    = 0x02;	// Select AD2 pin function
  LPC_IOCON->JTAG_nTRST_PIO1_2  = 0x02;	// Select AD3 pin function
#ifdef __SWD_DISABLED
  LPC_IOCON->ARM_SWDIO_PIO1_3   = 0x02;	// Select AD4 pin function
#endif
  LPC_IOCON->PIO1_4             = 0x01;	// Select AD5 pin function
  LPC_IOCON->PIO1_10            = 0x01;	// Select AD6 pin function
  LPC_IOCON->PIO1_11            = 0x01;	// Select AD7 pin function

  LPC_ADC->CR = ( 0x01 << 0 ) |  /* SEL=1,select channel 0~7 on ADC0 */
	((SystemCoreClock/ADC_Clk-1)<<8) |  /* CLKDIV = Fpclk / 4500000 - 1 */ 
	( 0 << 16 ) | 		/* BURST = 0, no BURST, software controlled */
	( 0 << 17 ) |  		/* CLKS = 0, 11 clocks/10 bits */
	( 1 << 21 ) |  		/* PDN = 1, normal operation */
	( 0 << 22 ) |  		/* TEST1:0 = 00 */
	( 0 << 24 ) |  		/* START = 0 A/D conversion stops */
	( 0 << 27 );		/* EDGE = 0 (CAP/MAT singal falling,trigger A/D conversion) */ 

  /* If POLLING, no need to do the following */
#if ADC_INTERRUPT_FLAG
  NVIC_EnableIRQ(ADC_IRQn);
  LPC_ADC->INTEN = 0x1FF;		/* Enable all interrupts */
#endif
  return;
}

/*****************************************************************************
** Function name:		ADCRead
**
** Descriptions:		Read ADC channel
**
** parameters:			Channel number
** Returned value:		Value read, if interrupt driven, return channel #
** 
*****************************************************************************/
uint32_t ADCRead( uint8_t channelNum )
{
#if !ADC_INTERRUPT_FLAG
  uint32_t regVal, ADC_Data;
#endif

  /* channel number is 0 through 7 */
  if ( channelNum >= ADC_NUM )
  {
	channelNum = 0;		/* reset channel number to 0 */
  }
  LPC_ADC->CR &= 0xFFFFFF00;
  LPC_ADC->CR |= (1 << 24) | (1 << channelNum);	
				/* switch channel,start A/D convert */
#if !ADC_INTERRUPT_FLAG
  while ( 1 )			/* wait until end of A/D convert */
  {
    regVal = *(uint32_t *) ((&LPC_ADC->DR0) + (channelNum));
	/* read result of A/D conversion */
	if ( regVal & ADC_DONE )
	{
	  break;
	}
  }	
        
  LPC_ADC->CR &= 0xF8FFFFFF;	/* stop ADC now */    
  if ( regVal & ADC_OVERRUN )	/* save data when it's not overrun, otherwise, return zero */
  {
	return ( 0 );
  }
  ADC_Data = ( regVal >> 6 ) & 0x3FF;
  return ( ADC_Data );	/* return A/D conversion value */
#else
  return ( channelNum );	/* if it's interrupt driven, the ADC reading is 
							done inside the handler. so, return channel number */
#endif
}

/*****************************************************************************
** Function name:		ADC0BurstRead
**
** Descriptions:		Use burst mode to convert multiple channels once.
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void ADCBurstRead( void )
{
  if ( LPC_ADC->CR & (0x7<<24) )
  {
	LPC_ADC->CR &= ~(0x7<<24);
  }
  /* Test channel 5,6,7 using burst mode because they are not shared
  with the JTAG pins. */
  LPC_ADC->CR &= ~0xFF;
  /* Read all channels, 0 through 7. */
  LPC_ADC->CR |= (0xFF);
  LPC_ADC->CR |= (0x1<<16);		/* Set burst mode and start A/D convert */
  return;						/* the ADC reading is done inside the 
								handler, return 0. */
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
