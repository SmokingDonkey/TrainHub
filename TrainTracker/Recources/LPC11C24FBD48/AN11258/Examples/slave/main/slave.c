/**********************************************************************
* $Id$		slave.c			2012-05-04
*//**
* @file		slave.c	
* @brief	Main program for slave.
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
#include "slave.h"
#include "sbl_slave.h"

#define SBL_SLAVE_PWR_LED_PORT      (LPC_GPIO2)
#define SBL_SLAVE_PWR_LED_PIN       (2)

#ifdef __MCU_LPC17xx
#define GPIO_SetDirOut(port,pin)          (port->FIODIR |= (1<<pin))
#define LED_OFF(port,pin)                 (port->FIOCLR |= (1<<pin))
#define LED_ON(port,pin)                 (port->FIOSET |= (1<<pin))
#elif defined (__MCU_LPC11xx)
#define GPIO_SetDirOut(port,pin)          (port->DIR |= (1<<pin))
#define LED_ON(port,pin)                 (port->DATA |= (1<<pin))
#define LED_OFF(port,pin)                (port->DATA &= ~(1<<pin))
#endif

/************************** PRIVATE VARIABLES *************************/
/*********************************************************************//**
 * @brief       Entry to main program
 * @param[in]   none
 * @return      int
 **********************************************************************/
int c_entry(void)
{
    Bool upgrade = FALSE;
    uint32_t* versions;
    /* Get Version ID */
    versions = (uint32_t*)SBL_SLV_FIRMWARE_ID_ADDR;
    if(*versions == 0xFFFFFFFF)
        upgrade = TRUE;
    // GPIO setting
    GPIO_SetDirOut(SBL_SLAVE_PWR_LED_PORT,SBL_SLAVE_PWR_LED_PIN);

    if(upgrade)
    {
        // LED ON
        LED_ON(SBL_SLAVE_PWR_LED_PORT,SBL_SLAVE_PWR_LED_PIN);

        /* Initialize */
        SBL_SlaveInit();

        while(1)
        {
            SBL_SlaveEntry();
        }
        
    }
    // LED OFF
    LED_OFF(SBL_SLAVE_PWR_LED_PORT,SBL_SLAVE_PWR_LED_PIN);
    // Run user code
    SBL_SlaveRunUserCode();
        
    /* Loop forever */
    while(1);
}

/* With ARM and GHS toolsets, the entry point is main() - this will
   allow the linker to generate wrapper code to setup stacks, allocate
   heap area, and initialize and copy code and data segments. For GNU
   toolsets, the entry point is through __start() in the crt0_gnu.asm
   file, and that startup code will setup stacks and data */
int main(void)
{
    return c_entry();
}


