/**********************************************************************
* $Id$        master.c            2012-05-04
*//**
* @file       master.c
* @brief      Main program for SBL master.
* @version    1.0
* @date       04. May. 2012
* @author     NXP MCU SW Application Team
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

#include "master.h"
#include "sbl_master.h"
#include "includes.h"


/************************** PRIVATE VARIABLES *************************/
extern void SystemCoreClockUpdate (void);

/*********************************************************************//**
 * @brief        Print Welcome menu
 * @param[in]    none
 * @return       None
 **********************************************************************/
void print_menu(void)
{
#if defined (__MCU_LPC17xx)
    _PRINT_("\r\nSBL Master Demo\r\nMCU: LPC17xx");
#elif defined (__MCU_LPC11xx)
    _PRINT_("\r\nSBL Master Demo\r\nMCU: LPC11xx");
#else
    #error "\r\nPlease define MCU used!"
#endif
    _PRINT_("Commands:");
    _PRINT("    \'r': Read Firmware Version ID\r\n" \
            "    \'u': Upgrade User Application Firmware\r\n"  \
            "    \'s': Upgrade Secondary Boot Loader Firmware\r\n");
}

/*********************************************************************//**
 * @brief        Print block which will be updated in Slave
 * @param[in]    BlockNum
 * @return       None
 **********************************************************************/
void StartSendingBlock(uint32_t BlockNum)
{
   _PRINT(".");
}

/* With ARM and GHS toolsets, the entry point is main() - this will
   allow the linker to generate wrapper code to setup stacks, allocate
   heap area, and initialize and copy code and data segments. For GNU
   toolsets, the entry point is through __start() in the crt0_gnu.asm
   file, and that startup code will setup stacks and data */
int main(void)
{
    SBL_FirmVerion_Type firm_vers;
    SBL_RET_CODE     ret;
    SBL_CB_Type callbacks;
    uint8_t ch;

    SystemCoreClockUpdate();
    DebugInit();
    print_menu();

    // Register Callback
    callbacks.UpgradeStartPFN = StartSendingBlock;
    // SBL Init
    SBL_MasterInit();
    while(1)
    {
        _PRINT("\r\n>");
        ch = _GET_C;
        _PRINT_C(ch);_PRINT_("");
        switch(ch)
        {
            case 'r':       // read firmware ID
                _PRINT_("Read Firmware Version ID...");
                ret = SBL_MasterReadFwVersionID(&firm_vers);
                if(ret == SBL_OK)
                {
                    _PRINT_("Firmware Version:");
                    _PRINT("    Major: ");_PRINT_H(firm_vers.Major);
                    _PRINT("\r\n    Minor: ");_PRINT_H(firm_vers.Minor);
                    _PRINT("\r\n    Rev  : ");_PRINT_H(firm_vers.Revision);
                    _PRINT_("");
                }
                break;
            case 'u':         // upgrade firmware
            case 'U':
                // upgrade firmware for slave
                _PRINT("Upgrade User Application Firmware");
                ret = SBL_MasterUpgradeFirmware(FALSE,&callbacks);
                _PRINT_("");
                if(ret == SBL_OK)
                {
                    _PRINT_("\r\nDONE");
                }
                break;
            case 's':       // upgrade sector0
                // upgrade firmware for slave
                _PRINT("Upgrade SBL Firmware");
                ret = SBL_MasterUpgradeFirmware(TRUE,&callbacks);
                if(ret == SBL_OK)
                {
                    _PRINT_("\r\nDONE");
                }
                break;
            default: 
                continue;
        }
        if(ret != SBL_OK)
        {
             _PRINT_(" Error!!!!");    
        }
    }
    // Deinit SPI
    //SBL_DeInit();
}




