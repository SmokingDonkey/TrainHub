/**********************************************************************
* $Id$		mcu_main_new.c			2012-05-04
*//**
* @file		mcu_main_new.c
* @brief	User program.
* @version	1.0
* @date		04. May. 2012
* @author	NXP MCU SW Application Team
* 
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
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
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/

#include "includes.h"

#define VTOR_OFFSET         (0x00001000)

SBL_FirmVerion_Type firm_vers __attribute__((section("firmware_id_section")))= {0,1,'b'};

/************************** PRIVATE DEFINITIONS *************************/
#ifdef __MCU_LPC17xx
#define MCB_1700
//#define IAR_LPC_1768
#elif defined (__MCU_LPC11xx)
#define MCB_1100
#endif

#if defined (MCB_1700) 
 /* Number of user LEDs */
#define LED_NUM     4
const unsigned long led_mask[] = { 1<<3, 1<<4, 1<<5, 1<<6 };
#elif defined(MCB_1100)
#define LED_NUM     4
const unsigned long led_pin[] = { 3, 4, 5, 6 };
#endif

/************************** PRIVATE VARIABLES *************************/
/* SysTick Counter */
volatile unsigned long SysTickCnt;

/************************** PRIVATE FUNCTIONS *************************/
void SysTick_Handler (void);
void ReAllocateNVIC(void);

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief            SysTick handler sub-routine (1ms)
 * @param[in]        None
 * @return           None
 **********************************************************************/
void SysTick_Handler (void) {
  SysTickCnt++;
}

/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************//**
 * @brief            Re-allocate vector interrupt table
 * @param[in]        None
 * @return           int
 **********************************************************************/
void ReAllocateNVIC(void)
{
#ifdef __MCU_LPC17xx
    __disable_irq();
    NVIC_SetVTOR(VTOR_OFFSET);
    __enable_irq();
#elif defined (__MCU_LPC11xx)
    uint32_t* src,*dst;
    int32_t size;

    __disable_irq();
    // copy vector table
    src = (uint32_t*)VTOR_OFFSET;
    dst = (uint32_t*)0x10000000;
    size = 192;

    while(size > 0)
    {
        *dst++ = *src++;
        size -= 4;
    }
     LPC_SYSCON->SYSMEMREMAP = 0x1;    /* remap to internal RAM */
    __enable_irq();
#endif
}
/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief          c_entry: Main program body
 * @param[in]      None
 * @return         None
 **********************************************************************/
void c_entry(void)
{   
    int num = 0;
    uint32_t systickcnt;
    SBL_SlaveInit();

    /* Relocate NVIC */
    ReAllocateNVIC();

    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock/1000 - 1); /* Generate interrupt each 1 ms   */

#if defined (MCB_1700) 
    GPIO_SetDir(2, 0x0000007C, 1);           /* LEDs on PORT2 defined as Output    */
    GPIO_ClearValue(2, 0x0000007C);
#elif defined(MCB_1100)
    for(num = LED_NUM-1;num>=0;num--)
    {
        GPIO_SetDir(2, led_pin[num],1);
        GPIO_SetValue(2, led_pin[num]);
    }
#elif defined(IAR_LPC_1768)
    GPIO_SetDir(1, (1<<25), 1);
    GPIO_ClearValue(1, (1<<25));
#endif

    while(1)
    {
        if(SBL_SlaveCmdRecv())
        {
            SBL_SlaveCmdHandler(SBL_SlaveGetRecvCmd());
        }
        else
        {
            if(((SysTickCnt - systickcnt) < 500))
            {
#if defined (MCB_1700)
                for(num = LED_NUM-1;num>=0;num--)
                    GPIO_SetValue(2, led_mask[num]);
#elif defined(MCB_1100)   
                for(num = LED_NUM-1;num>=0;num--)
                    GPIO_SetValue(2, led_pin[num]);
#else
                  GPIO_SetValue(2,(1<<25));
#endif
            }
            else if(((SysTickCnt - systickcnt) < 1000))
            { 
#ifdef MCB_1700              
                for(num = LED_NUM-1;num>=0;num--)
                    GPIO_ClearValue(2, led_mask[num]);
#elif defined(MCB_1100)   
                for(num = LED_NUM-1;num>=0;num--)
                    GPIO_ClearValue(2, led_pin[num]);
#else
                GPIO_ClearValue(1,(1<<25));
#endif    
            }
            else
            {
                 systickcnt = SysTickCnt;
            }
        }
        
    }
    //SBL_DeInit();
}

/* With ARM and GHS toolsets, the entry point is main() - this will
   allow the linker to generate wrapper code to setup stacks, allocate
   heap area, and initialize and copy code and data segments. For GNU
   toolsets, the entry point is through __start() in the crt0_gnu.asm
   file, and that startup code will setup stacks and data */
int main(void)
{
    c_entry();
    return 0;
}

